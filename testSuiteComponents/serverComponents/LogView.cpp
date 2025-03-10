// LogView.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "LogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LogView

IMPLEMENT_DYNCREATE(LogView, CScrollView)

LogView::LogView()
{
}

LogView::~LogView()
{
}


BEGIN_MESSAGE_MAP(LogView, CScrollView)
	//{{AFX_MSG_MAP(LogView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LogView drawing

void LogView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void LogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// LogView diagnostics

#ifdef _DEBUG
void LogView::AssertValid() const
{
	CScrollView::AssertValid();
}

void LogView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// LogView message handlers
