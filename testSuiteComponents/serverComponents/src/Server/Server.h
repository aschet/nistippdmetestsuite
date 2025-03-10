// Server.h : main header file for the SERVER application
//

#if !defined(AFX_SERVER_H)
#define AFX_SERVER_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*1 #ifndef __AFXWIN_H__
//*1  	#error include 'stdafx.h' before including this file for PCH
//*1 #endif
#include <afxwin.h>         //*1 MFC core and standard components
#include <afxext.h>         //*1 MFC extensions
#include <afxdisp.h>        //*1 MFC Automation classes
#include <afxdtctl.h>		//*1 MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT  //*1
#include <afxcmn.h>			//*1 MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT //*1

#include <afxsock.h>		//*1 MFC socket extensions

#include "../../resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CServerApp:
// See Server.cpp for the implementation of this class
//

class CServerApp : public CWinApp
{
public:
	CServerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


// Implementation

	//{{AFX_MSG(CServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVER_H)
