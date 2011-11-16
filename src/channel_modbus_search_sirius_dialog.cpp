#include "channel_modbus_search_sirius_dialog.h"
#include "../dependency/DataServerEngine/include/utils/types.h"

IMPLEMENT_DYNAMIC_CLASS(NPORT_DeviceSearch, wxDialog)

NPORT_DeviceSearch::NPORT_DeviceSearch()
{

}

NPORT_DeviceSearch::NPORT_DeviceSearch( wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos /*= wxDefaultPosition*/, const wxSize & size /*= wxDefaultSize*/, long style /*= wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION*/ )
{
	Create(parent, id, title, pos, size, style);
}

NPORT_DeviceSearch::~NPORT_DeviceSearch()
{

}

bool NPORT_DeviceSearch::Create( wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos /*= wxDefaultPosition*/, const wxSize & size /*= wxDefaultSize*/, long style /*= wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION*/, const wxString & name /*= _T("Обзор серверов")*/ )
{
	if ( !wxDialog::Create(parent, id, title, pos, size, style, name) )
	{
		return false;
	}

	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* main_sizer = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* list_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* text_sizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* btn_sizer = new wxBoxSizer(wxVERTICAL);

	main_sizer->Add(list_sizer, 0, wxEXPAND | wxALL, 10);
	main_sizer->Add(text_sizer, 0, wxEXPAND | wxALL, 10);
	main_sizer->Add(btn_sizer, 0, wxEXPAND | wxALL, 10);

	m_serverView = new wxListView(panel, wxID_ANY, wxDefaultPosition, wxSize(200, 100));
	wxListItem col0;
	col0.SetId(0);
	col0.SetText( wxT("Server") );
	col0.SetWidth(195);
	m_serverView->InsertColumn(0, col0);
	list_sizer->Add(m_serverView, 1, wxEXPAND | wxALL, 0);

	m_hostName = new wxTextCtrl(panel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(200, 20), wxSUNKEN_BORDER);
	text_sizer->Add(new wxStaticText(panel, wxID_ANY, wxT("IP адресс / Имя компьютера:") ), 0, wxTOP | wxBOTTOM, 2);
	text_sizer->Add(m_hostName, 0, wxEXPAND | wxALL, 0);

	m_btn_update = new wxButton( panel, ID_UpdateBtn, wxT("Обновить список") );
	Connect(ID_UpdateBtn, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NPORT_DeviceSearch::OnBrowseClicked));
	text_sizer->Add(m_btn_update, 0, wxEXPAND | wxALL, 10);

	m_btn_add = new wxButton( panel, ID_AddBtn, wxT("Добавить сервер") );
	Connect(ID_AddBtn, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(NPORT_DeviceSearch::OnAddClicked));
	text_sizer->Add(m_btn_add, 0, wxEXPAND | wxALL, 10);


	//main_sizer->SetSizeHints(panel);
	panel->SetSizer(main_sizer);

	SetSize(450, 300);

	return true;
}

void NPORT_DeviceSearch::OnBrowseClicked( wxCommandEvent& event )
{
	m_serverView->DeleteAllItems();

}

void NPORT_DeviceSearch::OnAddClicked( wxCommandEvent& event )
{
	wxDialog::EndModal(wxID_OK);
}