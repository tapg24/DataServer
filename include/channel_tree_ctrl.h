#ifndef CHANNEL_TREE_CTRL_H
#define CHANNEL_TREE_CTRL_H

#include "utils/types.h"
#include "channels/channel_base.h"

#include <boost/signals2.hpp>

#include <wx/frame.h>
#include <wx/treectrl.h>
#include <wx/treebase.h>
#include <wx/menu.h>

// resource
#include "connected.xpm"
#include "disconnected.xpm"
#include "inprogress.xpm"

class ChannelViewCtrl;

class ChannelTreeCtrl : public wxTreeCtrl
{
	DECLARE_DYNAMIC_CLASS(ChannelTreeCtrl);

	enum
	{
		ID_CreateChannel,
		ID_DestroyChannel,
		ID_StartChannel,
		ID_StopChannel,
		ID_RenameChannel,
		ID_SearchDevice_SIRIUS
	};

	enum TreeCtrlIcon
	{
		TreeCtrlIcon_ChannelConnected = 0,
		TreeCtrlIcon_ChannelDisconnected,
		TreeCtrlIcon_ChannelInProgress
	};


public:
	ChannelTreeCtrl();
	ChannelTreeCtrl(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(250, 0), long style = wxTR_DEFAULT_STYLE|wxTR_HIDE_ROOT, const wxValidator &validator = wxDefaultValidator, const wxString &name = "");
	~ChannelTreeCtrl();

	bool Create(wxWindow *parent, wxWindowID id = wxID_ANY, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxSize(250, 0), long style = wxTR_DEFAULT_STYLE, const wxValidator &validator = wxDefaultValidator, const wxString &name = "");
	void UpdateBinding();
	void SetView(ChannelViewCtrl* view);

protected:
	void OnEditLabel(wxTreeEvent& event);
	void OnItemRigthClicked(wxTreeEvent& event);
	void OnPopupClick(wxMenuEvent& event);
	void OnItemSelChanged(wxTreeEvent& event);
	void OnChannelStateChangeProxy(const uint32_t id, Channels::ChannelState state);
	void OnChannelStateChange(wxCommandEvent& event);
	wxTreeItemId FindItem(wxTreeItemId root, const int channelId);
private:
	ChannelViewCtrl* view_;
	wxTreeItemId rootItem_;
	wxTreeItemId rootOPC_;
	wxTreeItemId rootModbusRTU_NPort_;
	wxTreeItemId clickedItem_;
	boost::signals2::connection channel_connection_;
};

class ChannelStateEvent
{
public:
	const uint32_t channelId;
	const Channels::ChannelState channelState;

	ChannelStateEvent(const uint32_t id, const Channels::ChannelState state)
		: channelId(id),
		channelState(state)
	{}
};

class ChannelData : public wxTreeItemData
{
	const int id_;

public:
	ChannelData(const int id) : id_(id) {};
	~ChannelData() {};

	bool operator==(const ChannelData &other) const { return id_ == other.GetId(); };

	const int GetId() const { return id_; };
};

#endif // CHANNEL_TREE_CTRL_H