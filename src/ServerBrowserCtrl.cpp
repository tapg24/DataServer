#include "core/project_mgr.h"
#include "entries/entry.h"
#include "entries/ts_def.h"
#include "servers/opc da/opc_server.h"
#include "ServerBrowserCtrl.h"
//#include "PropertyFrame.h"
#include "utils/variant.h"
#include "utils/string_algorithm.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

ServerTagBrowserCtrl::ServerTagBrowserCtrl(wxWindow* parent) :
		wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VIRTUAL),
			refreshTimer_(this)
{
	wxListItem col0; 
	col0.SetText( _("Имя тега") ); 
	col0.SetWidth(300);
	InsertColumn(0, col0); 

	wxListItem col1; 
	col1.SetText( _("Тип данных") ); 
	col1.SetWidth(100);
	InsertColumn(1, col1);

	wxListItem col2; 
	col2.SetText( _("Значение") );
	col2.SetWidth(300);
	InsertColumn(2, col2); 

	wxListItem col3; 
	col3.SetText( _("Качество") ); 
	col3.SetWidth(100);
	InsertColumn(3, col3);

	wxListItem col4; 
	col4.SetText( _("Временная метка") ); 
	col4.SetWidth(200);
	InsertColumn(4, col4);

	itemAttr_1.SetBackgroundColour( *wxWHITE );
	itemAttr_2.SetBackgroundColour( wxColor(240, 240, 240) );

	Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( ServerTagBrowserCtrl::OnItemSelected ) );
	Connect( wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler( ServerTagBrowserCtrl::OnItemRigthClicked ) );
	Connect( wxEVT_TIMER, wxTimerEventHandler( ServerTagBrowserCtrl::Refresh ) );
	//Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( ServerBrowserCtrl::OnEraseBackground ) );

	refreshTimer_.Start(500);
}

ServerTagBrowserCtrl::~ServerTagBrowserCtrl()
{
	refreshTimer_.Stop();
}

void ServerTagBrowserCtrl::Refresh(wxTimerEvent& event)
{
	if ( opcServer_.expired() )
	{
		DeleteAllItems();
		return;
	}

	if ( filtredNames_.empty() )
	{
		DeleteAllItems();
		return;
	}

	SetItemCount( filtredNames_.size() );
	RefreshItems( 0, filtredNames_.size() - 1);
}

wxString ServerTagBrowserCtrl::OnGetItemText(long item, long column) const
{
	if ( opcServer_.expired() )
	{
		return wxEmptyString;
	}

	if (column == 0)
	{
		OPCServerSPtr opcServer = opcServer_.lock();
		const frl::opc::address_space::Tag* tag = opcServer->GetTag( filtredNames_[ item ] );
		Channels::CacheItemPtr cacheItem( new Channels::CacheItem( tag->getID(), tag->getTimeStamp(), tag->getQuality(), tag->read() ) );

		itemCache_[0] = cacheItem->GetName();
		itemCache_[1] = cacheItem->GetTypeAsString();
		if ( boost::ends_with(cacheItem->GetName(), "_TS") )
		{
			itemCache_[2] = TS_STATE_HOLDER::GET_VALUES_AS_STRING( cacheItem->GetValue().AsUShort() );
		}
		else
		{
			itemCache_[2] = cacheItem->GetValueAsString();
		}
		itemCache_[3] = cacheItem->GetQualityAsString();
		itemCache_[4] = cacheItem->GetTimestampAsString();
	}

	return itemCache_[ column ];
}

wxListItemAttr* ServerTagBrowserCtrl::OnGetItemAttr(long item) const
{
	return (item%2) ? &itemAttr_1 : &itemAttr_2;
}

void ServerTagBrowserCtrl::OnEraseBackground(wxEraseEvent & event)
{
	// to prevent flickering, erase only content *outside* of the 
	// actual list items stuff
	if(GetItemCount() > 0)
	{
		wxDC * dc = event.GetDC();
		assert(dc);

		// get some info
		wxCoord width = 0, height = 0;
		GetClientSize(&width, &height);

		wxCoord x, y, w, h;
		dc->SetClippingRegion(0, 0, width, height);
		dc->GetClippingBox(&x, &y, &w, &h); 


		long top_item = GetTopItem();
		long bottom_item = top_item + GetCountPerPage();
		if( bottom_item >= GetItemCount() )
		{
			bottom_item = GetItemCount() - 1;
		}                

		// trick: we want to exclude a couple pixels
		// on the left side thus use wxLIST_RECT_LABEL
		// for the top rect and wxLIST_RECT_BOUNDS for bottom
		// rect
		wxRect top_rect, bottom_rect;
		GetItemRect(top_item, top_rect, wxLIST_RECT_LABEL);
		GetItemRect(bottom_item, bottom_rect, wxLIST_RECT_BOUNDS);

		// set the new clipping region and do erasing
		wxRect items_rect(top_rect.GetLeftTop(), bottom_rect.GetBottomRight());
		wxRegion reg(wxRegion(x, y, w, h)); 
		reg.Subtract(items_rect);
		dc->DestroyClippingRegion();
		dc->SetClippingRegion(reg);

		// do erasing
		dc->SetBackground(wxBrush(GetBackgroundColour(), wxSOLID));
		dc->Clear();

		// restore old clipping region
		dc->DestroyClippingRegion();
		dc->SetClippingRegion(wxRegion(x, y, w, h));
	}
	else
	{
		event.Skip();
	}
}

wxString ServerTagBrowserCtrl::GetCellContentsString( long row_number, int column ) 
{
   wxListItem     row_info;  
   wxString       cell_contents_string;
 
   // Set what row it is (m_itemId is a member of the regular wxListCtrl class)
   row_info.m_itemId = row_number;
   // Set what column of that row we want to query for information.
   row_info.m_col = column;
   // Set text mask
   row_info.m_mask = wxLIST_MASK_TEXT;
 
   // Get the info and store it in row_info variable.   
   GetItem( row_info );
 
   // Extract the text out that cell
   cell_contents_string = row_info.m_text; 
 
   return cell_contents_string;
}


void ServerTagBrowserCtrl::OnItemRigthClicked( wxListEvent& event )
{
	if ( OPCServerSPtr opcServer = opcServer_.lock() )
	{
		wxMenu popup_menu;
		//popup_menu.Append(ID_DelTag, "Удалить");
		popup_menu.Append(ID_ShowProperty, "Свойства");
		popup_menu.Connect(wxEVT_COMMAND_MENU_SELECTED, (wxObjectEventFunction)&ServerTagBrowserCtrl::OnPopupClick, NULL, this);
		PopupMenu(&popup_menu);
	}
}

void ServerTagBrowserCtrl::OnPopupClick( wxCommandEvent& event )
{
	long itemIndex = -1;
	itemIndex = GetNextItem(itemIndex, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
	if ( itemIndex == -1)
	{
		return;
	}

	if ( OPCServerSPtr opcServer = opcServer_.lock() )
	{
		switch( event.GetId() )
		{
		case ID_DelTag:
			{
				if ( wxMessageBox("Удалить тег?", "Подтверждение", wxYES_NO, this) == wxYES )
				{
					//clientThread->DeleteTag( GetItemText(itemIndex) );
					wxTimerEvent event;
					Refresh(event);
				}
				break;
			}
		case ID_ShowProperty:
			{
				//PropertyFrame* propertyFrame = new PropertyFrame(m_mainFrame, wxID_ANY, wxT("Свойства тега"));
				// TODO: refactor access to data from opc server
				//propertyFrame->Init( projectMgr::getInstance().GetServerThread(), GetItemText(itemIndex).GetData(), true );
				//propertyFrame->Init( m_mainFrame->GetServerThread(), GetItemText(itemIndex).GetData(), true );
				//propertyFrame->Show(true);
				break;
			}
		}
	}
}

void ServerTagBrowserCtrl::UpdateBinding()
{
	opcServer_ = projectMgr::getInstance().WeakOpcServer();
	OPCServerSPtr opcServer = opcServer_.lock();
	opcServer->GetAllLeafs( allNames_, NULL );
}

void ServerTagBrowserCtrl::SetFilter( const string_t& filter )
{
	filter_ = filter;
	filtredNames_.clear();
	BOOST_FOREACH(const string_t& str, allNames_)
	{
		if ( boost::contains( str, filter_ ) )
		{
			filtredNames_.push_back(str);
		}
	}

	Refresh( wxTimerEvent() );
}

size_t ServerTagBrowserCtrl::GetFilterCount()
{
	return filtredNames_.size();
}

void ServerTagBrowserCtrl::OnItemSelected( wxListEvent& event )
{
	signal_OnItemSelected_(filtredNames_.at(event.GetIndex()));
}

boost::signals2::connection ServerTagBrowserCtrl::BindTo_OnItemSelected( boost::function<void(const string_t&)> slot)
{
	return signal_OnItemSelected_.connect(slot);
}

OpcServerBrowserCtrl::OpcServerBrowserCtrl( wxWindow* parent ) : wxPanel(parent)
{
	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* filterSizer = new wxBoxSizer(wxHORIZONTAL);
	filter_ = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(300, 20));
	Connect( wxEVT_COMMAND_TEXT_UPDATED, wxTextEventHandler( OpcServerBrowserCtrl::view_OnFilterUpdated ) );
	filterSizer->Add(filter_, 0, wxALL, 5);

	tagCountText_ = new wxStaticText(this, wxID_ANY, wxT(""));
	filterSizer->Add(tagCountText_, 0, wxALL, 5);
	
	mainSizer->Add(filterSizer, 0, wxALL, 0);

	wxBoxSizer* tagSizer = new wxBoxSizer(wxHORIZONTAL);
	view_ = new ServerTagBrowserCtrl(this);
	tagSizer->Add(view_, 1, wxGROW, 5);

	infoPanel_ = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(300, 0));
	infoPanel_->SetMinSize(wxSize(300, 0));
	infoPanel_->SetSizer(new wxBoxSizer(wxVERTICAL));
	tagSizer->Add(infoPanel_ , 0, wxEXPAND, 5);
	tagSizer->Layout();
	
	mainSizer->Add(tagSizer, 1, wxEXPAND | wxALL, 5);

	this->SetSizer(mainSizer);
}

void OpcServerBrowserCtrl::UpdateBinding()
{
	connection_onItemSelected_ = view_->BindTo_OnItemSelected( boost::bind(&OpcServerBrowserCtrl::view_OnItemSelected, this, _1) );
	view_->UpdateBinding();
	view_->SetFilter( string_t(filter_->GetValue().mb_str()) );
	UpdateInfo();
}

void OpcServerBrowserCtrl::view_OnFilterUpdated( wxCommandEvent& event )
{
	view_->SetFilter( string_t(filter_->GetValue().mb_str()) );
	UpdateInfo();
}

void OpcServerBrowserCtrl::UpdateInfo()
{
	string_t count = "Количество тегов: " + boost::lexical_cast<string_t>(view_->GetFilterCount());
	tagCountText_->SetLabel( count );
}

void OpcServerBrowserCtrl::view_OnItemSelected( const string_t& name )
{
	infoPanel_->GetSizer()->DeleteWindows();
	OPCServerSPtr opcServer = projectMgr::getInstance().SharedOpcServer();
	try
	{
		Channels::EntryPtr entry = opcServer->GetEntriesMgr().GetEntryByDst(name);
		wxString label = "Зависит от:\r\n";
		infoPanel_->GetSizer()->Add(new wxStaticText(infoPanel_, wxID_ANY, wxT("Зависит от:")), 0, wxALL, 2);
		BOOST_FOREACH(const string_t& src, entry->GetSrcList())
		{
			infoPanel_->GetSizer()->Add(new InfoCtrl(infoPanel_, src), 0, wxALL, 5);
		}
	}
	catch (Channels::EntriesMgr::DstNotFound& exception)
	{
		infoPanel_->GetSizer()->Add(new wxStaticText(infoPanel_, wxID_ANY, wxT("Пусто")), 0, wxALL, 5);
	}
	infoPanel_->GetSizer()->Layout();
}


InfoCtrl::InfoCtrl( wxWindow* parent, const string_t& src ) :
	wxStaticText(parent, wxID_ANY, wxT("")), timer_(this), src_(src)
{
	wxTimerEvent event;
	Refresh(event);

	Connect( wxEVT_TIMER, wxTimerEventHandler( InfoCtrl::Refresh ) );
	timer_.Start(500);
}

void InfoCtrl::Refresh( wxTimerEvent& event )
{
	wxString text = src_;
	if ( projectMgr::getInstance().GetCacheMgr()->ItemExist(src_) )
	{
		text << " | " << projectMgr::getInstance().GetCacheMgr()->GetItem(src_)->GetValueAsString();
		text << " | " << projectMgr::getInstance().GetCacheMgr()->GetItem(src_)->GetQualityAsString();
	}
	else
	{
		text << " не существует";
	}

	SetLabel(text);
}

InfoCtrl::~InfoCtrl()
{

}
