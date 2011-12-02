#ifndef SERVER_BROWSER_H
#define SERVER_BROWSER_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/timer.h>
#include <wx/button.h>

#include "../dependency/DataServerEngine/include/utils/string.h"
#include "../dependency/DataServerEngine/include/servers/opc da/opc_server_fwd.h"

#include <vector>
#include <map>

#include <boost/signals2.hpp>

//class ItemAddedEvt;

class ServerTagBrowserCtrl : public wxListCtrl
{
	enum
	{
		ID_DelTag = wxID_HIGHEST,
		ID_ShowProperty
	};

	boost::signals2::signal< void(const string_t&) > signal_OnItemSelected_;

public:
	ServerTagBrowserCtrl(wxWindow* parent);
	~ServerTagBrowserCtrl();

	wxString OnGetItemText(long item, long column) const;
	wxListItemAttr* OnGetItemAttr(long item) const;
	wxString GetCellContentsString( long row_number, int column );
	void Refresh(wxTimerEvent& event);
	void OnItemSelected(wxListEvent& event);
	void OnItemRigthClicked(wxListEvent& event);
	void OnPopupClick(wxCommandEvent& event);
	void UpdateBinding();
	void SetFilter(const string_t& filter);
	size_t GetFilterCount();
	boost::signals2::connection BindTo_OnItemSelected(boost::function<void(const string_t&)> slot);

private:
	void OnEraseBackground(wxEraseEvent & event);

	OPCServerWPtr opcServer_;
	std::vector<string_t> allNames_;
	std::vector<string_t> filtredNames_;
	string_t filter_;
	mutable std::map<long, string_t> itemCache_;
	mutable wxListItemAttr itemAttr_1;
	mutable wxListItemAttr itemAttr_2;
	wxTimer refreshTimer_;
};

class InfoCtrl : public wxStaticText
{
public:
	InfoCtrl(wxWindow* parent, const string_t& src);
	~InfoCtrl();

protected:
	void Refresh(wxTimerEvent& event);

private:
	const string_t src_;
	wxTimer timer_;
};

class OpcServerBrowserCtrl : public wxPanel
{
public:
	OpcServerBrowserCtrl(wxWindow* parent);

	void UpdateBinding();

protected:
	void UpdateInfo();
	void view_OnFilterUpdated(wxCommandEvent& event);
	void view_OnItemSelected(const string_t& name);

private:
	ServerTagBrowserCtrl* view_;
	wxStaticText* tagCountText_;
	wxPanel* infoPanel_;
	wxTextCtrl* filter_;
	boost::signals2::connection connection_onItemSelected_;
};

#endif // SERVER_BROWSER_H