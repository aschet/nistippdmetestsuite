#if !defined(AFX_VIEWSTATUS_H__B3018ADE_2028_40F0_A987_BEE5D45A8E5A__INCLUDED_)
#define AFX_VIEWSTATUS_H__B3018ADE_2028_40F0_A987_BEE5D45A8E5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// viewStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// viewStatus view

class viewStatus : public CScrollView
{
protected:
	viewStatus();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(viewStatus)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(viewStatus)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~viewStatus();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(viewStatus)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWSTATUS_H__B3018ADE_2028_40F0_A987_BEE5D45A8E5A__INCLUDED_)
