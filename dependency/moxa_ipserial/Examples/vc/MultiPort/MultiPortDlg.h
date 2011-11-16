// MultiPortDlg.h : header file
//

#if !defined(AFX_MULTIPORTDLG_H__42683824_87F9_440B_A517_E7EC2AB4AE37__INCLUDED_)
#define AFX_MULTIPORTDLG_H__42683824_87F9_440B_A517_E7EC2AB4AE37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMultiPortDlg dialog

class CMultiPortDlg : public CDialog
{
// Construction
public:
	int port_id1;
	int port_id2;
	void GetInputData(void);
	CMultiPortDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiPortDlg)
	enum { IDD = IDD_MULTIPORT_DIALOG };
	CEdit	m_Receiver2;
	CEdit	m_Receiver1;
	CEdit	m_Transmitter2;
	CEdit	m_Transmitter1;
	CEdit	m_ServerPort2Ctrl;
	CEdit	m_ServerPort1Ctrl;
	CEdit	m_ServerIP2Ctrl;
	CEdit	m_ServerIP1Ctrl;
	CButton	m_OpenCtrl;
	CButton	m_CloseCtrl;
	CString	m_ServerIP1;
	CString	m_ServerIP2;
	int		m_ServerPort2;
	int		m_ServerPort1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiPortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMultiPortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOpen();
	afx_msg void OnClose();
	afx_msg void OnChangeTransmitter1();
	afx_msg void OnChangeTransmitter2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPORTDLG_H__42683824_87F9_440B_A517_E7EC2AB4AE37__INCLUDED_)
