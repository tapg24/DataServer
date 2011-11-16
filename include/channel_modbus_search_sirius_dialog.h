#ifndef NPORT_SEARCH_SIRIUS_DIALOG_H
#define NPORT_SEARCH_SIRIUS_DIALOG_H

#include <../dependency/DataServerEngine/include/channels/channel_modbus/modbus_rtu_nport_impl.h>

#include <wx/wx.h>
#include <wx/listctrl.h>

class NPORT_DeviceSearch : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(NPORT_DeviceSearch);

	enum
	{
		ID_UpdateBtn = wxID_HIGHEST,
		ID_AddBtn
	};

public:
	NPORT_DeviceSearch();
	NPORT_DeviceSearch(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION);
	~NPORT_DeviceSearch();

	bool Create(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION, const wxString & name = _T("Îבחמנ סונגונמג"));

private:
	void OnBrowseClicked(wxCommandEvent& event);
	void OnAddClicked(wxCommandEvent& event);

	ModbusRTU_NPORT_Impl modbusStream_;

	wxListView* m_serverView;
	wxTextCtrl* m_hostName;
	wxButton* m_btn_update;
	wxButton* m_btn_add;
};


#endif // NPORT_SEARCH_SIRIUS_DIALOG_H