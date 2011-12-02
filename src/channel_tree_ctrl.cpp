#include "channel_tree_ctrl.h"
#include "channel_view_ctrl.h"

#include "../dependency/DataServerEngine/include/channels/channel_opc/channel_opc.h"
#include "channel_opc_dialog.h"
#include "nport_device_search_dialog.h"
#include "../dependency/DataServerEngine/include/core/project_mgr.h"
#include "../dependency/DataServerEngine/include/channels/channel_mgr.h"
#include "../dependency/DataServerEngine/include/channels/cache_item.h"

IMPLEMENT_DYNAMIC_CLASS(ChannelTreeCtrl, wxTreeCtrl);

const wxEventType ChannelStateEvt = wxNewEventType();

ChannelTreeCtrl::ChannelTreeCtrl()
{
}

ChannelTreeCtrl::ChannelTreeCtrl( wxWindow *parent, wxWindowID id /*= wxID_ANY*/, const wxPoint &pos /*= wxDefaultPosition*/, const wxSize &size /*= wxDefaultSize*/, long style /*= wxTR_DEFAULT_STYLE*/, const wxValidator &validator /*= wxDefaultValidator*/, const wxString &name /*= ""*/ )
{
	Create(parent, id, pos, size, style, validator, name);
}

ChannelTreeCtrl::~ChannelTreeCtrl()
{
	channel_connection_.disconnect();
}

bool ChannelTreeCtrl::Create( wxWindow *parent, wxWindowID id /*= wxID_ANY*/, const wxPoint &pos /*= wxDefaultPosition*/, const wxSize &size /*= wxDefaultSize*/, long style /*= wxTR_DEFAULT_STYLE*/, const wxValidator &validator /*= wxDefaultValidator*/, const wxString &name /*= ""*/ )
{
	if ( !wxTreeCtrl::Create(parent, id, pos, size, style, validator, name) )
	{
		return false;
	}

	wxImageList *images = new wxImageList(16, 16, true);
	images->Add(wxIcon(connected_xpm));
	images->Add(wxIcon(disconnected_xpm));
	images->Add(wxIcon(inprogress_xpm));
	this->SetImageList(images);
	AssignImageList(images);

	rootItem_ = AddRoot("root item");
	rootOPC_ = AppendItem(rootItem_, "OPC");
	rootModbusRTU_NPort_ = AppendItem(rootItem_, "Modbus RTU NPort");

	Connect( wxEVT_COMMAND_TREE_ITEM_RIGHT_CLICK, wxTreeEventHandler( ChannelTreeCtrl::OnItemRigthClicked ) );
	Connect( wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler( ChannelTreeCtrl::OnItemSelChanged ) );
	Connect( wxEVT_COMMAND_TREE_END_LABEL_EDIT, wxTreeEventHandler( ChannelTreeCtrl::OnEditLabel ) );
	Connect( ChannelStateEvt, wxCommandEventHandler( ChannelTreeCtrl::OnChannelStateChange ));

	return true;
}

void ChannelTreeCtrl::SetView(ChannelViewCtrl* view)
{
	view_ = view;
}

wxTreeItemId ChannelTreeCtrl::FindItem( wxTreeItemId root, const int channelId )
{
	wxTreeItemIdValue cookie;
	wxTreeItemId search;
	wxTreeItemId item = GetFirstChild( root, cookie );
	wxTreeItemId child;
 
	while( item.IsOk() )
	{
		if( ChannelData* channelData = dynamic_cast<ChannelData*>( GetItemData(item) ) )
		{
			if (channelId == channelData->GetId())
			{
				return item;
			}
		}
	
		if( ItemHasChildren( item ) )
		{
			wxTreeItemId search = FindItem( item, channelId );
			if( search.IsOk() )
			{
				return search;
			}
		}
		item = GetNextChild( root, cookie);
	}
 
	/* Not found */
	wxTreeItemId dummy;
	return dummy;
}

void ChannelTreeCtrl::OnEditLabel(wxTreeEvent& event)
{
}

void ChannelTreeCtrl::OnItemRigthClicked( wxTreeEvent& event )
{
	clickedItem_ = boost::cref( event.GetItem() );

	wxMenu popup_menu;

	if ( clickedItem_ == rootOPC_ )
	{
		popup_menu.Append(ID_CreateChannel, "Добавить канал");
	}
	else if ( clickedItem_ == rootModbusRTU_NPort_ )
	{
		popup_menu.Append(ID_CreateChannel, "Добавить канал");
		popup_menu.Append(ID_SearchDevice_SIRIUS, "Поиск устройств");
	}
	else
	{
		wxTreeItemData* itemData = GetItemData(clickedItem_);
		ChannelData* channelData = dynamic_cast<ChannelData*>(itemData);
		const Channels::ChannelPtr channelPtr = projectMgr::getInstance().GetChannelMgr()->Get(channelData->GetId());
		if ( channelPtr->GetState() == Channels::Disconnected )
		{
			popup_menu.Append(ID_StartChannel, "Запустить");
		}
		else
		{
			popup_menu.Append(ID_StopChannel, "Остановить");
		}
		popup_menu.Append(ID_DestroyChannel, "Удалить канал");
		popup_menu.Append(ID_RenameChannel, "Переменовать");
	}

	popup_menu.Connect( wxEVT_COMMAND_MENU_SELECTED, wxMenuEventHandler( ChannelTreeCtrl::OnPopupClick ), NULL, this );
	PopupMenu(&popup_menu);
}

void ChannelTreeCtrl::OnPopupClick( wxMenuEvent& event )
{
	switch( event.GetId() )
	{
	case ID_CreateChannel :
		{
			if ( clickedItem_ == rootOPC_ )
			{
				ChannelOPCDialog dlg(this, wxID_ANY, "Обзор");
				ChannelOPCDialog::DialogParamPtr dialogParam( new ChannelOPCDialog::DialogParam() );
				dlg.InitParam(dialogParam);
				if ( wxID_OK == dlg.ShowModal() )
				{
					Channels::ChannelPtr channel = projectMgr::getInstance().GetChannelMgr()->CreateOPC("123123123", dialogParam->hostName, dialogParam->serverName );
					channel->Start();
				}
				else
				{
					return;
				}
			}
			else if ( clickedItem_ == rootModbusRTU_NPort_ )
			{
				//Channels::AbstractChannel* channel = channelMgr::getInstance().Create(Channels::MODBUS_RTU);

				//std::string initString;

				//channel->SetInitParam( initString );
				//channel->Start();
			}
			break;
		}
	case ID_DestroyChannel :
		{
			wxTreeItemData* itemData = GetItemData(clickedItem_);
			ChannelData* channelData = dynamic_cast<ChannelData*>(itemData);
			projectMgr::getInstance().GetChannelMgr()->Detach(channelData->GetId());

			break;
		}
	case ID_StartChannel :
		{
			wxTreeItemData* itemData = GetItemData(clickedItem_);
			ChannelData* channelData = dynamic_cast<ChannelData*>(itemData);
			Channels::ChannelPtr channel = projectMgr::getInstance().GetChannelMgr()->Get(channelData->GetId());
			channel->Start();

			break;
		}
	case ID_StopChannel :
		{
			wxTreeItemData* itemData = GetItemData(clickedItem_);
			ChannelData* channelData = dynamic_cast<ChannelData*>(itemData);
			Channels::ChannelPtr channel = projectMgr::getInstance().GetChannelMgr()->Get(channelData->GetId());
			channel->Stop();

			break;
		}
	case ID_RenameChannel :
		{
			EditLabel(clickedItem_);
			break;
		}
	case ID_SearchDevice_SIRIUS :
		{
			DeviceSearch_NPort deviceSearchDlg(this, wxID_ANY, "Поиск");
			deviceSearchDlg.ShowModal();
			break;
		}

	}
}

void ChannelTreeCtrl::OnChannelStateChangeProxy(const Channels::ChannelInfo info)
{
	wxCommandEvent evt( ChannelStateEvt );
	evt.SetClientData( new ChannelStateEvent(info) );
	this->AddPendingEvent(evt);
}

void ChannelTreeCtrl::OnChannelStateChange( wxCommandEvent& event )
{
	std::auto_ptr<ChannelStateEvent> eventData( reinterpret_cast<ChannelStateEvent*>(event.GetClientData()) );

	switch( eventData->info.state )
	{
	case Channels::Created:
		{
			wxTreeItemId curItem;

			if ( eventData->info.type == Channels::OPC_DA2 )
			{
				curItem = rootOPC_;
			}
			else if ( eventData->info.type == Channels::MODBUS_RTU_NPORT )
			{
				curItem = rootModbusRTU_NPort_;
			}

			wxTreeItemId addedItem = AppendItem(curItem, eventData->info.name);
			SetItemImage(addedItem, TreeCtrlIcon_ChannelDisconnected);
			SetItemData(addedItem, new ChannelData(eventData->info.id));
			SelectItem(addedItem);

			break;
		}
	case Channels::Deleted:
		{
			wxTreeItemId item = FindItem(rootItem_, eventData->info.id);
			BOOST_ASSERT( item );
			Delete(item);

			break;
		}
	case Channels::Connected:
		{
			wxTreeItemId item = FindItem(rootItem_, eventData->info.id);
			BOOST_ASSERT( item );
			SetItemImage(item, TreeCtrlIcon_ChannelConnected);

			break;
		}
	case Channels::Disconnected:
		{
			wxTreeItemId item = FindItem(rootItem_, eventData->info.id);
			BOOST_ASSERT( item );
			SetItemImage(item, TreeCtrlIcon_ChannelDisconnected);

			break;
		}
	case Channels::InProgress:
		{
			wxTreeItemId item = FindItem(rootItem_, eventData->info.id);
			BOOST_ASSERT( item );
			SetItemImage(item, TreeCtrlIcon_ChannelInProgress);

			break;
		}
	case Channels::Renamed:
		{
			wxTreeItemId item = FindItem(rootItem_, eventData->info.id);
			BOOST_ASSERT( item );
			SetItemText(item, eventData->info.name);

			break;
		}
	}
}

void ChannelTreeCtrl::OnItemSelChanged(wxTreeEvent& event)
{
	clickedItem_ = boost::cref( event.GetItem() );

	if ( clickedItem_ == rootOPC_ || clickedItem_ == rootModbusRTU_NPort_ )
	{
		view_->Switch(0);
	}
	else
	{
		wxTreeItemData* itemData = GetItemData(clickedItem_);
		ChannelData* channelData = dynamic_cast<ChannelData*>(itemData);
		view_->Switch(channelData->GetId());
	}
}

void ChannelTreeCtrl::UpdateBinding()
{
	channel_connection_ = projectMgr::getInstance().GetChannelMgr()->Bind( boost::bind(&ChannelTreeCtrl::OnChannelStateChangeProxy, this, _1) );
}

void ChannelTreeCtrl::Clear()
{
	wxTreeCtrl::DeleteAllItems();
}
