#include "core/project_mgr.h"
#include "channels/cache_item.h"
#include "servers/opc da/opc_server.h"

#include <boost/foreach.hpp>

#include <wx/datetime.h>

#include "PropertyFrame.h"
#include "MainFrame.h"

IMPLEMENT_DYNAMIC_CLASS(PropertyFrame, wxFrame)

PropertyFrame::PropertyFrame()
{

}

PropertyFrame::PropertyFrame( wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos /*= wxDefaultPosition*/, const wxSize & size /*= wxDefaultSize*/, long style /*= wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION*/ )
{
	wxASSERT_MSG(false, "do refactoring");

	BOOST_ASSERT( parent != NULL );

	Create(parent, id, title, pos, size, style);
}

PropertyFrame::~PropertyFrame()
{
	MakeModal(false);
	m_parentFrame->SetFocus();
}

bool PropertyFrame::Create( wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos /*= wxDefaultPosition*/, const wxSize & size /*= wxDefaultSize*/, long style /*= wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION*/, const wxString & name /*= _T("Свойства тега")*/ )
{
	if ( !wxFrame::Create(parent, id, title, pos, size, style, name) )
	{
		return false;
	}

	m_parentFrame = wxDynamicCast(parent, MainFrame);
	BOOST_ASSERT( m_parentFrame != NULL );

	MakeModal(true);

	////Connect( ID_Update, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MainFrame::OnBrowseClicked) );
	////Connect( ID_TagList, wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxCommandEventHandler( AddTagFrame::OnItemRigthClicked ) );

	return true;
}

void PropertyFrame::Init( OPCServerWPtr innerOpcServer, const std::string& tagName, bool fromServer )
{
	/*m_innerOpcServer = innerOpcServer;
	m_tagName = tagName;

	if ( boost::shared_ptr<OPCServerThread> sp_serverThread = innerOpcServer.lock() )
	{
		long rowId = 0;

		if ( !fromServer )
		{
			ClientView();
			std::vector<TagConvertInfo> convertInfoVector = sp_serverThread->GetConvertInfo(m_tagName);

			BOOST_FOREACH(const TagConvertInfo& convertInfo, convertInfoVector)
			{
				tagList->InsertItem(0, convertInfo.serverTagName_);
	
				if ( convertInfo.stateMaskMap_.empty() )
				{
					tagList->SetItem(rowId, 2, "Direct");
				}
				else
				{
					tagList->SetItem(rowId, 2, "with state mask map...");
				}
			}
		}
		else
		{
			ServerView();
			std::vector<std::string> clientTags = sp_serverThread->ServerToClientTags(m_tagName);

			BOOST_FOREACH(const wxString& name, clientTags)
			{
				tagList->InsertItem(0, name);
				Channels::CacheItemPtr cacheItem = projectMgr::getInstance().CacheMgr()->GetItem( name.c_str() );
				tagList->SetItem(rowId, 1, cacheItem->GetValueAsString());
			}
		}
	}*/
}

void PropertyFrame::ClientView()
{
	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	tagList = new wxListCtrl(panel, ID_PropertyList, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

	wxListItem col0;
	col0.SetId(0);
	col0.SetText( _("Зависящие теги") ); 
	col0.SetWidth(200);
	tagList->InsertColumn(0, col0);

	wxListItem col1; 
	col1.SetId(1);
	col1.SetText( _("Значение") ); 
	col1.SetWidth(50);
	tagList->InsertColumn(1, col1);

	wxListItem col2; 
	col2.SetId(2);
	col2.SetText( _("Конвертирование") ); 
	col2.SetWidth(50);
	tagList->InsertColumn(2, col2);

	mainSizer->Add(tagList, 1, wxEXPAND | wxALL, 10);

	panel->SetSizer(mainSizer);

	SetSize(450, 300);
}

void PropertyFrame::ServerView()
{
	wxPanel* panel = new wxPanel(this);

	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	tagList = new wxListCtrl(panel, ID_PropertyList, wxDefaultPosition, wxDefaultSize, wxLC_REPORT);

	wxListItem col0;
	col0.SetId(0);
	col0.SetText( _("Зависит от") ); 
	col0.SetWidth(200);
	tagList->InsertColumn(0, col0);

	wxListItem col1; 
	col1.SetId(1);
	col1.SetText( _("Значение") ); 
	col1.SetWidth(50);
	tagList->InsertColumn(1, col1);

	wxListItem col2; 
	col2.SetId(2);
	col2.SetText( _("Конвертирование") ); 
	col2.SetWidth(50);
	tagList->InsertColumn(2, col2);

	mainSizer->Add(tagList, 1, wxEXPAND | wxALL, 10);

	panel->SetSizer(mainSizer);

	SetSize(450, 300);

}
