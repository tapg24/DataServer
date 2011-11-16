// MultiPortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiPort.h"
#include "MultiPortDlg.h"

#include "IPSerial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int port1_opened = 0;			// 0: close 1:open
int port2_opened = 0;			// 0: close 1:open
HANDLE hRequestExitEvent = FALSE;
HANDLE hThreads[2];
DWORD dwThreadId;

CMultiPortDlg *pointer;


DWORD WINAPI ThreadEditBoard( LPVOID param );

DWORD WINAPI ThreadEditBoard(LPVOID param)
{
	char szStr[2048];
	CString strTemp;
	CString strResult;
	int t = (int) param;
	int ret;
	while( TRUE )
	{
		switch( t )
		{
		case 0:
			if( (ret = nsio_read(pointer->port_id1, szStr, 2048) )> 0)
			{
				pointer->m_Receiver1.GetWindowText(strTemp);
				strResult.Format("%s%s", strTemp, szStr);
				pointer->m_Receiver1.SetWindowText(strResult);
			}
			break;
		case 1:
			if( (ret = nsio_read(pointer->port_id2, szStr, 2048) )> 0)
			{
				pointer->m_Receiver2.GetWindowText(strTemp);
				strResult.Format("%s%s", strTemp, szStr);
				pointer->m_Receiver2.SetWindowText(strResult);
			}
			break;
		default:
			break;
		}
		if( WaitForSingleObject(hRequestExitEvent, 0) != WAIT_TIMEOUT)
		{
			return (DWORD) -1;
		}
		Sleep(10);
	}

	return 0;

}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiPortDlg dialog

CMultiPortDlg::CMultiPortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiPortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiPortDlg)
	m_ServerIP1 = _T("");
	m_ServerIP2 = _T("");
	m_ServerPort2 = 0;
	m_ServerPort1 = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMultiPortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiPortDlg)
	DDX_Control(pDX, IDC_RECEIVER2, m_Receiver2);
	DDX_Control(pDX, IDC_RECEIVER1, m_Receiver1);
	DDX_Control(pDX, IDC_TRANSMITTER2, m_Transmitter2);
	DDX_Control(pDX, IDC_TRANSMITTER1, m_Transmitter1);
	DDX_Control(pDX, IDC_SERVER_PORT2, m_ServerPort2Ctrl);
	DDX_Control(pDX, IDC_SERVER_PORT1, m_ServerPort1Ctrl);
	DDX_Control(pDX, IDC_SERVER_IP2, m_ServerIP2Ctrl);
	DDX_Control(pDX, IDC_SERVER_IP1, m_ServerIP1Ctrl);
	DDX_Control(pDX, IDC_OPEN, m_OpenCtrl);
	DDX_Control(pDX, IDC_CLOSE, m_CloseCtrl);
	DDX_Text(pDX, IDC_SERVER_IP1, m_ServerIP1);
	DDX_Text(pDX, IDC_SERVER_IP2, m_ServerIP2);
	DDX_Text(pDX, IDC_SERVER_PORT2, m_ServerPort2);
	DDV_MinMaxInt(pDX, m_ServerPort2, 0, 65536);
	DDX_Text(pDX, IDC_SERVER_PORT1, m_ServerPort1);
	DDV_MinMaxInt(pDX, m_ServerPort1, 0, 65536);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMultiPortDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiPortDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_EN_CHANGE(IDC_TRANSMITTER1, OnChangeTransmitter1)
	ON_EN_CHANGE(IDC_TRANSMITTER2, OnChangeTransmitter2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiPortDlg message handlers

BOOL CMultiPortDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
/*m_ServerIP1 = "192.168.4.105";
m_ServerIP2 = "192.168.4.105";
m_ServerPort1 = 1;
m_ServerPort2 = 2;*/
	UpdateData(FALSE);
	pointer = this;

	m_OpenCtrl.EnableWindow(TRUE);
	m_CloseCtrl.EnableWindow(FALSE);
	m_ServerIP1Ctrl.EnableWindow(TRUE);
	m_ServerPort1Ctrl.EnableWindow(TRUE);
	m_ServerIP2Ctrl.EnableWindow(TRUE);
	m_ServerPort2Ctrl.EnableWindow(TRUE);
	m_Transmitter1.EnableWindow(FALSE);
	m_Transmitter2.EnableWindow(FALSE);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMultiPortDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMultiPortDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMultiPortDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMultiPortDlg::GetInputData()
{
	UpdateData(TRUE);
}

void CMultiPortDlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	GetInputData();
	char szTemp[128];
	sprintf(szTemp, m_ServerIP1);

	port_id1 = nsio_open(szTemp, m_ServerPort1, 3000);
	if( port_id1 >= 0 )
	{
		port1_opened = 1;
	}
	else
	{
		AfxMessageBox("Open 1st Port Fail");
		return;
	}

	sprintf(szTemp, m_ServerIP2);
	port_id2 = nsio_open(szTemp, m_ServerPort2, 3000);
	if( port_id2 >= 0 )
	{
		port2_opened = 1;
	}
	else
	{
		AfxMessageBox("Open 2nd Port Fail");
		port1_opened = 0;
		nsio_close(port_id1);
		return;
	}

	nsio_ioctl(port_id1, B38400, (BIT_8 | STOP_1 | P_IP_SERIAL_NONE));
	nsio_ioctl(port_id2, B38400, (BIT_8 | STOP_1 | P_IP_SERIAL_NONE));
	nsio_flowctrl (port_id1, F_NONE);
	nsio_flowctrl (port_id2, F_NONE);

	m_OpenCtrl.EnableWindow(FALSE);
	m_CloseCtrl.EnableWindow(TRUE);
	m_ServerIP1Ctrl.EnableWindow(FALSE);
	m_ServerPort1Ctrl.EnableWindow(FALSE);
	m_ServerIP2Ctrl.EnableWindow(FALSE);
	m_ServerPort2Ctrl.EnableWindow(FALSE);
	m_Transmitter1.EnableWindow(TRUE);
	m_Transmitter2.EnableWindow(TRUE);
	AfxMessageBox("Port is opened!");

	hRequestExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL );

	for(int i=0; i<2; i++)
		hThreads[i] = CreateThread(NULL, 0, ThreadEditBoard, (LPVOID) i, 0, &dwThreadId);

/*	HWND hWndEditBoard1 = GetSafeHwnd();
	AfxBeginThread( EditBoard1, hWndEditBoard1, THREAD_PRIORITY_NORMAL );

	HWND hWndEditBoard2 = GetSafeHwnd();
	AfxBeginThread( EditBoard2, hWndEditBoard2, THREAD_PRIORITY_NORMAL );
*/
}

void CMultiPortDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	SetEvent(hRequestExitEvent);
	WaitForMultipleObjects( 2, hThreads, TRUE, INFINITE);
	for( int i=0; i< 2; i++)
		CloseHandle(hThreads[i]);

	nsio_close(port_id1);
	nsio_close(port_id2);
	port1_opened = 0;
	port2_opened = 0;

	m_OpenCtrl.EnableWindow(TRUE);
	m_CloseCtrl.EnableWindow(FALSE);
	m_ServerIP1Ctrl.EnableWindow(TRUE);
	m_ServerPort1Ctrl.EnableWindow(TRUE);
	m_ServerIP2Ctrl.EnableWindow(TRUE);
	m_ServerPort2Ctrl.EnableWindow(TRUE);
	m_Transmitter1.EnableWindow(FALSE);
	m_Transmitter2.EnableWindow(FALSE);
	AfxMessageBox("Port is closed!");
}

void CMultiPortDlg::OnChangeTransmitter1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if( port1_opened == 1)
	{
		char szTemp[2048];
		int i;
		m_Transmitter1.GetWindowText(szTemp, 2048);
		i = m_Transmitter1.GetWindowTextLength();
		int ret = nsio_write(port_id1, &szTemp[i-1], 1 );
	}	
}

void CMultiPortDlg::OnChangeTransmitter2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if( port2_opened == 1)
	{
		char szTemp[2048];
		int i;
		m_Transmitter2.GetWindowText(szTemp, 2048);
		i = m_Transmitter2.GetWindowTextLength();
		int ret = nsio_write(port_id2, &szTemp[i-1], 1 );
	}	
}
