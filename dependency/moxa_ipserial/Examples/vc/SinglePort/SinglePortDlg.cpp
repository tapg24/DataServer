// SinglePortDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SinglePort.h"
#include "SinglePortDlg.h"

#include "InfoDlg.h"
#include "IPSerial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int port_opened = 0;			// 0: close 1:open

CSinglePortDlg *pointer;

UINT EditBoard( LPVOID param );

UINT EditBoard(LPVOID param)
{
	static int nCount=0;
	int ret;
	while( TRUE )
	{
		Sleep(1000);
		if( port_opened == 1)
		{
			if( nCount > 1)
			{
				ret = nsio_lstatus(pointer->port_id);
				if( ret >= 0)
				{
					if( ret & 0x01 )
					{
						pointer->m_CTSCtrl.SetWindowText("On");
					}
					else
					{
						pointer->m_CTSCtrl.SetWindowText("Off");
					}
					if( ret & 0x02 )
					{
						pointer->m_DSRCtrl.SetWindowText("On");
					}
					else
					{
						pointer->m_DSRCtrl.SetWindowText("Off");
					}
					if( ret & 0x08 )
					{
						pointer->m_DCDCtrl.SetWindowText("On");
					}
					else
					{
						pointer->m_DCDCtrl.SetWindowText("Off");
					}
				}
				nCount = 0;
			}
			else
				nCount++;

			char szStr[2048];
			CString strTemp;
			CString strResult;

			if( (ret = nsio_read(pointer->port_id, szStr, 2048) )> 0)
			{
				pointer->m_Transceiver.GetWindowText(strTemp);
				strResult.Format("%s%s", strTemp, szStr);
				pointer->m_Transceiver.SetWindowText(strResult);
			}

			ret = nsio_breakcount(pointer->port_id);
			pointer->TotalBreakCount += ret;
			strResult.Empty();
			strResult.Format("%d", pointer->TotalBreakCount);
			pointer->m_BreakCtrl.SetWindowText(strResult);

		}
		else 
			break;
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
// CSinglePortDlg dialog

CSinglePortDlg::CSinglePortDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSinglePortDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSinglePortDlg)
	m_DTR = -1;
	m_Port = 0;
	m_RTS = -1;
	m_ServerIP = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSinglePortDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSinglePortDlg)
	DDX_Control(pDX, IDC_DATA_STATUS, m_DataStatusCtrl);
	DDX_Control(pDX, IDC_RESET_SERVER, m_ResetServerCtrl);
	DDX_Control(pDX, IDC_BREAK_CTRL, m_BreakCtrl);
	DDX_Control(pDX, IDC_DSR_CTRL, m_DSRCtrl);
	DDX_Control(pDX, IDC_DCD_CTRL, m_DCDCtrl);
	DDX_Control(pDX, IDC_CTS_CTRL, m_CTSCtrl);
	DDX_Control(pDX, IDC_RESET_PORT, m_ResetPortCtrl);
	DDX_Control(pDX, IDC_SEND_BREAK, m_SendBreakCtrl);
	DDX_Control(pDX, IDC_OPEN, m_OpenCtrl);
	DDX_Control(pDX, IDC_CLOSE, m_CloseCtrl);
	DDX_Control(pDX, IDC_PORT, m_PortCtrl);
	DDX_Control(pDX, IDC_SERVER_IP, m_ServerIPCtrl);
	DDX_Control(pDX, IDC_TRANSCEIVER, m_Transceiver);
	DDX_Control(pDX, IDC_STOP_BITS, m_StopBits);
	DDX_Control(pDX, IDC_PARITY, m_Parity);
	DDX_Control(pDX, IDC_FLOW_CONTROL, m_FlowControl);
	DDX_Control(pDX, IDC_DATA_BITS, m_DataBits);
	DDX_Control(pDX, IDC_BAUD_RATE, m_BaudRate);
	DDX_Radio(pDX, IDC_DTR_ON, m_DTR);
	DDX_Text(pDX, IDC_PORT, m_Port);
	DDX_Radio(pDX, IDC_RTS_ON, m_RTS);
	DDX_Text(pDX, IDC_SERVER_IP, m_ServerIP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSinglePortDlg, CDialog)
	//{{AFX_MSG_MAP(CSinglePortDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_CLOSE, OnClose)
	ON_BN_CLICKED(IDC_RESET_PORT, OnResetPort)
	ON_BN_CLICKED(IDC_RESET_SERVER, OnResetServer)
	ON_BN_CLICKED(IDC_CHECK_ALIVE, OnCheckAlive)
	ON_BN_CLICKED(IDC_SEND_BREAK, OnSendBreak)
	ON_CBN_SELCHANGE(IDC_BAUD_RATE, OnSelchangeBaudRate)
	ON_CBN_SELCHANGE(IDC_DATA_BITS, OnSelchangeDataBits)
	ON_CBN_SELCHANGE(IDC_STOP_BITS, OnSelchangeStopBits)
	ON_CBN_SELCHANGE(IDC_PARITY, OnSelchangeParity)
	ON_CBN_SELCHANGE(IDC_FLOW_CONTROL, OnSelchangeFlowControl)
	ON_BN_CLICKED(IDC_DTR_ON, OnDtrOn)
	ON_BN_CLICKED(IDC_DTR_OFF, OnDtrOff)
	ON_BN_CLICKED(IDC_RTS_ON, OnRtsOn)
	ON_BN_CLICKED(IDC_RTS_OFF, OnRtsOff)
	ON_EN_CHANGE(IDC_TRANSCEIVER, OnChangeTransceiver)
	ON_BN_CLICKED(IDC_DATA_STATUS, OnDataStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSinglePortDlg message handlers

BOOL CSinglePortDlg::OnInitDialog()
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

//m_ServerIP = "192.168.4.105";				// Test Only
m_Port = 1;									// Test Only

	m_DTR = 0;					// Set default DTR on
	m_RTS = 0;					// Set default RTS on
	m_BaudRate.SetCurSel(13);	// Set default baud rate = 38400
	m_DataBits.SetCurSel(3);	// Set default data bits = 8 bits
	m_StopBits.SetCurSel(0);	// Set default stop bits = 1 bits
	m_Parity.SetCurSel(0);		// Set default parity = None
	m_FlowControl.SetCurSel(0);	// Set default flow control = None
	m_BaudRate.EnableWindow(FALSE);
	m_DataBits.EnableWindow(FALSE);
	m_StopBits.EnableWindow(FALSE);
	m_Parity.EnableWindow(FALSE);
	m_FlowControl.EnableWindow(FALSE);



	m_CTSCtrl.SetWindowText("Off");
	m_DSRCtrl.SetWindowText("Off");
	m_DCDCtrl.SetWindowText("Off");

	UpdateData(FALSE);
	pointer = this;

	m_ResetPortCtrl.EnableWindow(FALSE);
	m_SendBreakCtrl.EnableWindow(FALSE);
	m_CloseCtrl.EnableWindow(FALSE);
	TotalBreakCount = 0;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSinglePortDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSinglePortDlg::OnPaint() 
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
HCURSOR CSinglePortDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSinglePortDlg::GetInputData()
{
	UpdateData(TRUE);

}

// =====================================================================
void CSinglePortDlg::OnOpen() 
{
	// TODO: Add your control notification handler code here
	GetInputData();
	char szTemp[128];
	sprintf(szTemp, m_ServerIP);
	TotalBreakCount = 0;
	port_id = nsio_open( szTemp, m_Port, 3000);
	if( port_id >= 0 )
	{
		port_opened = 1;			// port is opened
		nsio_SetReadTimeouts(port_id, MAXDWORD);
		IOCtrl();

		m_ServerIPCtrl.EnableWindow(FALSE);
		m_PortCtrl.EnableWindow(FALSE);
		m_OpenCtrl.EnableWindow(FALSE);

		m_BaudRate.EnableWindow(TRUE);
		m_DataBits.EnableWindow(TRUE);
		m_StopBits.EnableWindow(TRUE);
		m_Parity.EnableWindow(TRUE);
		m_FlowControl.EnableWindow(TRUE);

		m_ResetPortCtrl.EnableWindow(TRUE);
		m_ResetServerCtrl.EnableWindow(FALSE);
		m_SendBreakCtrl.EnableWindow(TRUE);
		m_DataStatusCtrl.EnableWindow(TRUE);
		m_CloseCtrl.EnableWindow(TRUE);

		HWND hWndEditBoard = GetSafeHwnd();
		AfxBeginThread( EditBoard, hWndEditBoard, THREAD_PRIORITY_NORMAL );
	}
	else
		AfxMessageBox("Fail");
}

void CSinglePortDlg::OnClose() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_close(port_id);
		if( ret == NSIO_OK )
		{
			AfxMessageBox("OK");
			port_opened = 0;			// port is closed

			m_ServerIPCtrl.EnableWindow(TRUE);
			m_PortCtrl.EnableWindow(TRUE);
			m_OpenCtrl.EnableWindow(TRUE);

			m_BaudRate.EnableWindow(FALSE);
			m_DataBits.EnableWindow(FALSE);
			m_StopBits.EnableWindow(FALSE);
			m_Parity.EnableWindow(FALSE);
			m_FlowControl.EnableWindow(FALSE);

			m_ResetPortCtrl.EnableWindow(FALSE);
			m_ResetServerCtrl.EnableWindow(TRUE);
			m_SendBreakCtrl.EnableWindow(FALSE);
			m_CloseCtrl.EnableWindow(FALSE);

		}
		else
			AfxMessageBox("Fail");
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnResetPort() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_resetport(port_id, "");
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");	
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnResetServer() 
{
	// TODO: Add your control notification handler code here
	GetInputData();
	char szTemp[128];
	sprintf(szTemp, m_ServerIP);
	int ret = nsio_resetserver(szTemp, "1111");
	if( ret == NSIO_OK )
		AfxMessageBox("OK");
	else
		AfxMessageBox("Fail");	
}

void CSinglePortDlg::OnCheckAlive() 
{
	// TODO: Add your control notification handler code here
	GetInputData();
	char szTemp[128];
	CString strMessage;
	int ret1;
	sprintf(szTemp, m_ServerIP);
	strMessage.Empty();

	ret1 = nsio_checkalive(szTemp, 300);
	if( ret1 == NSIO_OK )
		strMessage = "Server is alive";
	else
		strMessage = "Server is not alive";

/*	if( port_opened == 1)
	{
		ret2 = nsio_checkport(port_id, 300);
		if( ret2 == NSIO_OK )
		strMessage = strMessage + "Port is alive!";
		else
		strMessage = strMessage + "Port is not alive!";
	}
	else
		strMessage = strMessage + "Port not open!";
*/
		AfxMessageBox(strMessage);


}

void CSinglePortDlg::OnSendBreak() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_break(port_id, 30);
	}
	else
		AfxMessageBox("Port not open!");
}


// =====================================================================

void CSinglePortDlg::OnSelchangeBaudRate() 
{
	// TODO: Add your control notification handler code here
	CString strTemp;
	long baudrate;
	if( port_opened == 1)
	{

		GetInputData();

		m_BaudRate.GetLBText( m_BaudRate.GetCurSel(), strTemp);
		baudrate = atol( strTemp );
		int ret = nsio_baud(port_id, baudrate);
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");	
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::IOCtrl()
{
	CString strTemp;
	int databits;
	int baud, mode=0;

	GetInputData();
	
	m_BaudRate.GetLBText( m_BaudRate.GetCurSel(), strTemp );
	switch( atol( strTemp ) )
	{
	case 50:		baud = B50;		break;
	case 75:		baud = B75;		break;
	case 110:		baud = B110;	break;
	case 134:		baud = B134;	break;
	case 150:		baud = B150;	break;
	case 300:		baud = B300;	break;
	case 600:		baud = B600;	break;
	case 1200:		baud = B1200;	break;
	case 2400:		baud = B2400;	break;
	case 4800:		baud = B4800;	break;
	case 7200:		baud = B7200;	break;
	case 9600:		baud = B9600;	break;
	case 19200:		baud = B19200;	break;
	case 38400:		baud = B38400;	break;
	case 57600:		baud = B57600;	break;
	case 115200:	baud = B115200;	break;
	case 230400:	baud = B230400;	break;
	case 460800:	baud = B460800;	break;
	case 921600:	baud = B921600;	break;
	default:		break;
	}

	m_StopBits.GetLBText( m_StopBits.GetCurSel(), strTemp );
	switch( atoi( strTemp ) )
	{
	case 1:			mode |= STOP_1;	break;
	case 2:			mode |= STOP_2;	break;
	default:		break;
	}

	switch( m_Parity.GetCurSel() )
	{
	case 0:			mode |= P_IP_SERIAL_NONE;	break;
	case 1:			mode |= P_IP_SERIAL_EVEN;	break;
	case 2:			mode |= P_IP_SERIAL_ODD;	break;
	case 3:			mode |= P_IP_SERIAL_MARK;	break;
	case 4:			mode |= P_IP_SERIAL_SPACE;	break;
	default:		break;
	}

	m_DataBits.GetLBText( m_DataBits.GetCurSel(), strTemp );
	databits = atoi( strTemp );

	switch( databits )
	{
	case 5:			mode |= BIT_5;	break;
	case 6:			mode |= BIT_6;	break;
	case 7:			mode |= BIT_7;	break;
	case 8:			mode |= BIT_8;	break;
	default:		break;
	}


	int ret = nsio_ioctl(port_id, baud, mode);
	if( ret == NSIO_OK )
		AfxMessageBox("OK");
	else
		AfxMessageBox("Fail");
}

void CSinglePortDlg::OnSelchangeDataBits() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		IOCtrl();
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnSelchangeStopBits() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		IOCtrl();
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnSelchangeParity() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		IOCtrl();
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnSelchangeFlowControl() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int mode=0;
		switch( m_FlowControl.GetCurSel() )
		{
		case 0:		mode = 0;		break;
		case 1:		mode = mode | 0x01 | 0x02;	break;
		case 2:		mode = mode | 0x04 | 0x08;	break;
		case 3:		mode = mode | 0x01 | 0x02 | 0x04 | 0x08;	break;
//		case 4:		mode |= 0x08;	break;
		default:	break;
		}
		int ret = nsio_flowctrl(port_id, mode);
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnDtrOn() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_DTR(port_id, 1);
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnDtrOff() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_DTR(port_id, 0);
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnRtsOn() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_RTS(port_id, 1);
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");
	}
	else
		AfxMessageBox("Port not open!");
}

void CSinglePortDlg::OnRtsOff() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_RTS(port_id, 0);
		if( ret == NSIO_OK )
			AfxMessageBox("OK");
		else
			AfxMessageBox("Fail");
	}
	else
		AfxMessageBox("Port not open!");
}
// =====================================================================


void CSinglePortDlg::OnChangeTransceiver() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		char szTemp[2048];
		int i;
		m_Transceiver.GetWindowText(szTemp, 2048);
		i = m_Transceiver.GetWindowTextLength();
		int ret = nsio_write(port_id, &szTemp[i-1], 1 );
	}
	else
		AfxMessageBox("Port not open!");
//    int ret = 0;
/*
	if( ret == (int) strlen(szTemp ) )
		AfxMessageBox("OK");
	else
		AfxMessageBox("Fail");
*/
}


void CSinglePortDlg::OnDataStatus() 
{
	// TODO: Add your control notification handler code here
	if( port_opened == 1)
	{
		int ret = nsio_data_status(port_id);
		if( ret < 0 )
			AfxMessageBox("Error");
		else
		{
			CString strResult;
			strResult.Empty();
			strResult.Format("Data Status = %X", ret);
			AfxMessageBox(strResult);
		}
	}
	else
		AfxMessageBox("Port not open!");	
}
