#include "../dependency/DataServerEngine/include/utils/string.h"
#include "../dependency/DataServerEngine/include/core/project_mgr.h"
#include "channel_tree_ctrl.h"
#include "channel_view_ctrl.h"

#include "MainFrame.h"
#include "ServerBrowserCtrl.h"
#include <wx/notebook.h>
#include <wx/filedlg.h>

#include <boost/log/trivial.hpp>

IMPLEMENT_DYNAMIC_CLASS(MainFrame, wxFrame)

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
	EVT_CLOSE(MainFrame::OnClose)
END_EVENT_TABLE()

MainFrame::MainFrame() : wxFrame(NULL, wxID_ANY,  wxT("Сервер сбора данных"), wxPoint(50,50), wxSize(800,600))
{
	//_crtBreakAlloc = 3888;

	//////////////////////////////////////////////////////////////////////////
	// menu
	//////////////////////////////////////////////////////////////////////////
	wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
	menuFile->Append(ID_LoadProject, _T("Открыть проект"));
	Connect(ID_LoadProject, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnLoadProjectClicked));
	menuFile->Append(ID_CloseProject, _T("Закрыть проект"));
	Connect(ID_CloseProject, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnCloseProjectClicked));
    menuFile->Append(ID_Exit, _T("Выход"));
	Connect(ID_Exit, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnExitClicked));
    menuBar->Append(menuFile, _T("Файл"));

	wxMenu *menuOPCServer = new wxMenu;
	menuOPCServer->Append(ID_RegServer, _T("Зарегистрировать OPC-сервер"));
	Connect(ID_RegServer, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnRegisterServerClicked));
	menuOPCServer->Append(ID_UnregServer, _T("Разрегистрировать OPC-сервер"));
	Connect(ID_UnregServer, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnUnregisterServerClicked));
	menuBar->Append(menuOPCServer, _T("OPC-сервер"));

    this->SetMenuBar(menuBar);


	wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this);

	wxNotebook* notebook = new wxNotebook(panel, wxID_ANY);
	wxPanel* pageClient = new wxPanel(notebook);
	wxPanel* pageServer = new wxPanel(notebook);
	notebook->AddPage(pageClient, "Источники данных");
	notebook->AddPage(pageServer, "OPC-сервер");

	sizer_main->Add(notebook, 1, wxEXPAND | wxALL, 10);

	//////////////////////////////////////////////////////////////////////////
	// client page
	//////////////////////////////////////////////////////////////////////////
	wxBoxSizer* sizer_client_page = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_client_list = new wxBoxSizer(wxHORIZONTAL);

	channelsTreeCtrl_ = new ChannelTreeCtrl(pageClient);

	sizer_client_list->Add(channelsTreeCtrl_, 0, wxEXPAND | wxALL, 10);

	channelsViewCtrl_ = new ChannelViewCtrl(pageClient);
	channelsTreeCtrl_->SetView(channelsViewCtrl_);
	sizer_client_list->Add(channelsViewCtrl_, 1, wxEXPAND | wxALL, 10);

	sizer_client_page->Add(sizer_client_list, 1, wxEXPAND | wxALL, 0);

	pageClient->SetSizer( sizer_client_page );

	//////////////////////////////////////////////////////////////////////////
	// server page
	//////////////////////////////////////////////////////////////////////////
	wxBoxSizer* sizer_server_page = new wxBoxSizer(wxVERTICAL);

	OpcServerBrowserCtrl_ = new OpcServerBrowserCtrl(pageServer);

	sizer_server_page->Add(OpcServerBrowserCtrl_, 1, wxEXPAND | wxALL, 10);

	pageServer->SetSizer( sizer_server_page );

	//////////////////////////////////////////////////////////////////////////
	// global sizer
	//////////////////////////////////////////////////////////////////////////
	sizer_main->SetSizeHints( panel );
	panel->SetSizer( sizer_main );

	SetSize(800, 600);
	Maximize();
	binding_ = projectMgr::getInstance().Bind( boost::bind(&MainFrame::UpdateUIBinding, this) );
}

MainFrame::~MainFrame()
{
	binding_.disconnect();
}

void MainFrame::OnExitClicked( wxCommandEvent& event )
{
	projectMgr::getInstance().CloseProject();
	this->Close(false);
}

void MainFrame::OnRegisterServerClicked( wxCommandEvent& event )
{
	projectMgr::getInstance().RegOpcServer();
}

void MainFrame::OnUnregisterServerClicked( wxCommandEvent& event )
{
	projectMgr::getInstance().UnregOpcServer();
}

void MainFrame::OnClose( wxCloseEvent& event )
{
	projectMgr::getInstance().CloseProject();
	this->Destroy();
}

void MainFrame::OnLoadProjectClicked( wxCommandEvent& event )
{
	wxFileDialog openFileDialog(this, wxT("Open file"), "", "", wxT("DataServer project|*.ds"), wxOPEN, wxDefaultPosition);
	if ( openFileDialog.ShowModal() == wxID_OK )
	{
		projectMgr::getInstance().LoadProject(openFileDialog.GetPath().mb_str());
	}
}

void MainFrame::OnCloseProjectClicked( wxCommandEvent& event )
{
	projectMgr::getInstance().CloseProject();
	this->SetLabel("Сервер сбора данных");
}

void MainFrame::UpdateUIBinding()
{
	channelsTreeCtrl_->UpdateBinding();
	OpcServerBrowserCtrl_->UpdateBinding();
	string_t label = projectMgr::getInstance().ProjectName() + " - Сервер сбора данных";
	this->SetLabel(label);
}

void MainFrame::LoadDefaultProject()
{
	try
	{
		projectMgr::getInstance().LoadProject();
	}
	catch (frl::Exception& exception)
	{
		wxString msg = exception.getFullDescription();
		BOOST_LOG_TRIVIAL(error) << msg.c_str();
		wxMessageBox(msg, "Exception");
	}
	catch (...)
	{
		BOOST_LOG_TRIVIAL(error) << "unhadled exception";
		wxMessageBox("unhadled exception", "unhadled exception");
	}
}
