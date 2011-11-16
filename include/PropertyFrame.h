#ifndef PROPERTYFRAME_H
#define PROPERTYFRAME_H

#include <string>
#include <boost/weak_ptr.hpp>

#include <wx/wx.h>
#include <wx/listctrl.h>

class MainFrame;
class OPCServerThread;

class PropertyFrame : public wxFrame
{

	DECLARE_DYNAMIC_CLASS(PropertyFrame);

public:
	PropertyFrame();
	PropertyFrame(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION);
	~PropertyFrame();

	bool Create(wxWindow* parent, wxWindowID id, const wxString & title, const wxPoint & pos = wxDefaultPosition, const wxSize & size = wxDefaultSize, long style = wxSYSTEM_MENU | wxCLOSE_BOX | wxCAPTION, const wxString & name = _T("Свойства тега"));
	void Init(OPCServerWPtr innerOpcServer, const std::string& tagName, bool fromServer);

private:
	wxString OnGetItemText(long item, long column) const;
	void Refresh(wxTimerEvent& event);
	void ClientView();
	void ServerView();
	//void OnItemRigthClicked(wxListEvent& event);
	//void OnPopupClick(wxCommandEvent& event);

private:
	MainFrame* m_parentFrame;
	boost::weak_ptr<OPCServerThread> m_innerOpcServer;
	std::string m_tagName;
	wxListCtrl* tagList;
};

enum
{
	ID_PropertyList = wxID_HIGHEST,
};

#endif // PROPERTYFRAME_H