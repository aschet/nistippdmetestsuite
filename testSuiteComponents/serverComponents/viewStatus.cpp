// viewStatus.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "viewStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// viewStatus

IMPLEMENT_DYNCREATE(viewStatus, CScrollView)

viewStatus::viewStatus()
{
}

viewStatus::~viewStatus()
{
}


BEGIN_MESSAGE_MAP(viewStatus, CScrollView)
	//{{AFX_MSG_MAP(viewStatus)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// viewStatus drawing

void viewStatus::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void viewStatus::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// viewStatus diagnostics

#ifdef _DEBUG
void viewStatus::AssertValid() const
{
	CScrollView::AssertValid();
}

void viewStatus::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// viewStatus message handlers
