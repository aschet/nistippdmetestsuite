// Test.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "Test.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Test dialog


Test::Test(CWnd* pParent /*=NULL*/)
	: CDialog(Test::IDD, pParent)
{
	//{{AFX_DATA_INIT(Test)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Test::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Test)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Test, CDialog)
	//{{AFX_MSG_MAP(Test)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Test message handlers
