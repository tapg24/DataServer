// SinglePort.h : main header file for the SINGLEPORT application
//

#if !defined(AFX_SINGLEPORT_H__086A8B5C_6FD0_4CE9_9B93_9149E4B86898__INCLUDED_)
#define AFX_SINGLEPORT_H__086A8B5C_6FD0_4CE9_9B93_9149E4B86898__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSinglePortApp:
// See SinglePort.cpp for the implementation of this class
//

class CSinglePortApp : public CWinApp
{
public:
	CSinglePortApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSinglePortApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSinglePortApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SINGLEPORT_H__086A8B5C_6FD0_4CE9_9B93_9149E4B86898__INCLUDED_)
