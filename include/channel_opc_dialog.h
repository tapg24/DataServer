#ifndef CHANNEL_OPC_DIALOG_H
#define CHANNEL_OPC_DIALOG_H

#include "utils/types.h"
#include "channels/channel_opc/channel_opc.h"

#include <wx/wx.h>
#include <wx/listctrl.h>

#include "../dependency/opc_client/include/OPCHost.h"
#include "../dependency/opc_client/include/OPCClient.h"

class ChannelOPCDialog : public wxDialog
{
	DECLARE_DYNAMIC_CLASS(ChannelOPCDialog);

	struct DialogParam
	{
		std::string hostName;
		std::string serverName;
	};

	typedef boost::shared_ptr<DialogParam> DialogParamPtr;

	enum
	{
		ID_UpdateBtn = wxID_HIGHEST,
		ID_AddBtn
	};

public:
	ChannelOPCDialog();
	ChannelOPCDialog(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION);
	~ChannelOPCDialog();

	bool Create(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION, const wxString & name = _T("Îבחמנ סונגונמג"));
	void InitParam(DialogParamPtr& paramPtr);

private:
	void OnBrowseClicked(wxCommandEvent& event);
	void OnAddClicked(wxCommandEvent& event);

	DialogParamPtr dialogParam;

	wxListView* m_serverView;
	wxTextCtrl* m_hostName;
	wxButton* m_btn_update;
	wxButton* m_btn_add;
};

#endif // CHANNEL_OPC_DIALOG_H