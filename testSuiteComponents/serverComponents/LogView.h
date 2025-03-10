#if !defined(AFX_LOGVIEW_H__AD0D9DD0_9101_4D16_8108_DBBAE47F5BFB__INCLUDED_)
#define AFX_LOGVIEW_H__AD0D9DD0_9101_4D16_8108_DBBAE47F5BFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LogView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LogView view

class LogView : public CScrollView
{
protected:
	LogView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(LogView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LogView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~LogView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(LogView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGVIEW_H__AD0D9DD0_9101_4D16_8108_DBBAE47F5BFB__INCLUDED_)
