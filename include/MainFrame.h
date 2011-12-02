#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include <wx/wx.h>

#include <boost/signals2.hpp>

class ChannelTreeCtrl;
class ChannelViewCtrl;
class ServerTagBrowserCtrl;
class OpcServerBrowserCtrl;

class MainFrame : public wxFrame
{
	DECLARE_DYNAMIC_CLASS(MainFrame);
	DECLARE_EVENT_TABLE()

public:
	MainFrame();
	~MainFrame();

	void LoadDefaultProject();
	void UpdateUIBinding();

private:
	void OnRegisterServerClicked(wxCommandEvent& event);
	void OnUnregisterServerClicked(wxCommandEvent& event);
	void OnExitClicked(wxCommandEvent& event);
	void OnLoadProjectClicked(wxCommandEvent& event);
	void OnCloseProjectClicked(wxCommandEvent& event);
	void OnClose(wxCloseEvent& event);

	ChannelTreeCtrl* channelsTreeCtrl_;
	ChannelViewCtrl* channelsViewCtrl_;
	OpcServerBrowserCtrl* OpcServerBrowserCtrl_;
	boost::signals2::connection binding_;
};

enum
{
	ID_Exit,
	ID_LoadProject,
	ID_CloseProject,
	ID_RegServer,
	ID_UnregServer,
};

#endif // MAIN_FRAME_H_