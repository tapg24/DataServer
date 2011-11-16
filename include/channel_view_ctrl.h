#ifndef CHANNEL_VIEW_CTRL
#define CHANNEL_VIEW_CTRL

#include "channels/cache_item.h"

#include "utils/types.h"

#include <string>
#include <vector>

#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/timer.h>

class ChannelViewCtrl : public wxListCtrl
{
	DECLARE_DYNAMIC_CLASS(ChannelViewCtrl);

public:
	ChannelViewCtrl();
	ChannelViewCtrl(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxLC_REPORT | wxLC_VIRTUAL, const wxValidator &validator = wxDefaultValidator, const wxString &name = "");
	~ChannelViewCtrl();

	bool Create(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxLC_REPORT | wxLC_VIRTUAL, const wxValidator &validator = wxDefaultValidator, const wxString &name = "");

	void Switch(const int id);
	void ChannelViewCtrl::Refresh(wxTimerEvent& event);
	wxString ChannelViewCtrl::OnGetItemText(long item, long column) const;
private:
	int m_channelId;
	std::vector<std::string> m_itemNames;
	wxTimer m_timerRefresh;
	mutable Channels::CacheItemPtr cacheItem;
};

#endif // CHANNEL_VIEW_CTRL