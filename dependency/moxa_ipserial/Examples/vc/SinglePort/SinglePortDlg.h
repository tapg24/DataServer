// SinglePortDlg.h : header file
//

#if !defined(AFX_SINGLEPORTDLG_H__BC4624A0_3E43_475E_814E_28D24A788E38__INCLUDED_)
#define AFX_SINGLEPORTDLG_H__BC4624A0_3E43_475E_814E_28D24A788E38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSinglePortDlg dialog

class CSinglePortDlg : public CDialog
{
// Construction
public:
	int m_DataStatus;
	void IOCtrl(void);
	int port_id;
	void GetInputData(void);
	CSinglePortDlg(CWnd* pParent = NULL);	// standard constructor

	int TotalBreakCount;
// Dialog Data
	//{{AFX_DATA(CSinglePortDlg)
	enum { IDD = IDD_SINGLEPORT_DIALOG };
	CButton	m_DataStatusCtrl;
	CButton	m_ResetServerCtrl;
	CEdit	m_BreakCtrl;
	CEdit	m_DSRCtrl;
	CEdit	m_DCDCtrl;
	CEdit	m_CTSCtrl;
	CButton	m_ResetPortCtrl;
	CButton	m_SendBreakCtrl;
	CButton	m_OpenCtrl;
	CButton	m_CloseCtrl;
	CEdit	m_PortCtrl;
	CEdit	m_ServerIPCtrl;
	CEdit	m_Transceiver;
	CComboBox	m_StopBits;
	CComboBox	m_Parity;
	CComboBox	m_FlowControl;
	CComboBox	m_DataBits;
	CComboBox	m_BaudRate;
	int		m_DTR;
	int		m_Port;
	int		m_RTS;
	CString	m_ServerIP;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSinglePortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSinglePortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnClose();
	afx_msg void OnResetPort();
	afx_msg void OnResetServer();
	afx_msg void OnCheckAlive();
	afx_msg void OnSendBreak();
	afx_msg void OnSelchangeBaudRate();
	afx_msg void OnSelchangeDataBits();
	afx_msg void OnSelchangeStopBits();
	afx_msg void OnSelchangeParity();
	afx_msg void OnSelchangeFlowControl();
	afx_msg void OnDtrOn();
	afx_msg void OnDtrOff();
	afx_msg void OnRtsOn();
	afx_msg void OnRtsOff();
	afx_msg void OnChangeTransceiver();
	afx_msg void OnDataStatus();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEPORTDLG_H__BC4624A0_3E43_475E_814E_28D24A788E38__INCLUDED_)
