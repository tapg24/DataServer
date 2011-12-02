#include "channel_view_ctrl.h"
#include "../dependency/DataServerEngine/include/core/project_mgr.h"
#include "../dependency/DataServerEngine/include/channels/channel_mgr.h"
#include "../dependency/DataServerEngine/include/channels/cache_mgr.h"

IMPLEMENT_DYNAMIC_CLASS(ChannelViewCtrl, wxListCtrl);

ChannelViewCtrl::ChannelViewCtrl()
{
}

ChannelViewCtrl::ChannelViewCtrl( wxWindow *parent, wxWindowID id /*= wxID_ANY*/, const wxPoint &pos /*= wxDefaultPosition*/, const wxSize &size /*= wxDefaultSize*/, long style /*= wxLC_REPORT | wxLC_VIRTUAL*/, const wxValidator &validator /*= wxDefaultValidator*/, const wxString &name /*= ""*/ )
{
	Create(parent, id, pos, size, style, validator, name);
}


ChannelViewCtrl::~ChannelViewCtrl()
{
}

bool ChannelViewCtrl::Create( wxWindow *parent, wxWindowID id /*= wxID_ANY*/, const wxPoint &pos /*= wxDefaultPosition*/, const wxSize &size /*= wxDefaultSize*/, long style /*= wxLC_REPORT | wxLC_VIRTUAL*/, const wxValidator &validator /*= wxDefaultValidator*/, const wxString &name /*= ""*/ )
{
	if ( !wxListCtrl::Create(parent, id, pos, size, style, validator, name) )
	{
		return false;
	}

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
	col2.SetWidth(150);
	InsertColumn(2, col2); 

	wxListItem col3; 
	col3.SetText( _("Качество") ); 
	col3.SetWidth(100);
	InsertColumn(3, col3);

	wxListItem col4; 
	col4.SetText( _("Временная метка") ); 
	col4.SetWidth(150);
	InsertColumn(4, col4);

	Connect( wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( ChannelViewCtrl::OnEraseBackground ) );

	m_timerRefresh.SetOwner(this);
	Connect( wxEVT_TIMER, wxTimerEventHandler( ChannelViewCtrl::Refresh ) );
	m_timerRefresh.Start(500);

	return true;
}

void ChannelViewCtrl::OnEraseBackground(wxEraseEvent & event)
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

void ChannelViewCtrl::Switch(const int id)
{
	m_channelId = id;
	if( projectMgr::getInstance().GetChannelMgr()->Exist(m_channelId) )
	{
		m_itemNames = projectMgr::getInstance().GetChannelMgr()->Get(m_channelId)->GetTagNames();
		if( m_itemNames.empty() )
		{
			m_itemNames.clear();
			DeleteAllItems();
			return;
		}

		wxTimerEvent event;
		Refresh(event);
	}
	else
	{
		m_itemNames.clear();
		DeleteAllItems();
	}
}

void ChannelViewCtrl::Refresh(wxTimerEvent& event)
{
	SetItemCount( m_itemNames.size() );
	RefreshItems( 0, m_itemNames.size() - 1);
}


wxString ChannelViewCtrl::OnGetItemText(long item, long column) const
{
	switch (column)
	{
	case 0:
		cacheItem = projectMgr::getInstance().GetCacheMgr()->GetItem( m_itemNames[item] );
		return cacheItem->GetName();
	case 1:
		return cacheItem->GetTypeAsString();
	case 2:
		return cacheItem->GetValueAsString();
	case 3:
		return cacheItem->GetQualityAsString();
	case 4:
		return cacheItem->GetTimestampAsString();
	default:
		return "wrong";
	}
}