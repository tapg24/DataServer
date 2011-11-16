#ifndef NPORT_SEARCH_SIRIUS_DIALOG_H
#define NPORT_SEARCH_SIRIUS_DIALOG_H

#include "utils/types.h"

#include <boost/shared_ptr.hpp>
#include <boost/thread.hpp>

#include <wx/wx.h>
#include <wx/listctrl.h>

class ModbusRTU_NPort;

class DeviceSearch_NPort : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(DeviceSearch_NPort);

	enum
	{
		ID_UpdateBtn = wxID_HIGHEST,
		ID_AddBtn
	};

public:
	DeviceSearch_NPort();
	DeviceSearch_NPort(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION);
	~DeviceSearch_NPort();

	bool Create(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION, const wxString & name = _T("Îבחמנ סונגונמג"));

private:
	void OnStartClicked(wxCommandEvent& event);
	void OnAddClicked(wxCommandEvent& event);
	void SearchDeviceThread();

	boost::shared_ptr<ModbusRTU_NPort> modbusStream_;
	boost::thread searchThread;

	wxListView* m_deviceView;
	wxTextCtrl* m_ipAdress;
	wxTextCtrl* m_portId;
	wxTextCtrl* m_baudRate;
	wxTextCtrl* m_addrMin;
	wxTextCtrl* m_addrMax;
	wxTextCtrl* m_pause;
	wxButton* m_startBtn;
	wxButton* m_btn_add;
};


#endif // NPORT_SEARCH_SIRIUS_DIALOG_H