#if !defined(AFX_COORDDLG_H__D1FA294B_9C7C_11D5_BAE6_005004188CC7__INCLUDED_)
#define AFX_COORDDLG_H__D1FA294B_9C7C_11D5_BAE6_005004188CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// coordDlg.h : header file
//


#define FROM_FILE 100
#define MEASUREMENT 200
#define INPUT 300
#define NONE 400

/////////////////////////////////////////////////////////////////////////////
// coordDlg dialog

class coordDlg : public CDialog
{
// Construction
public:
	coordDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(coordDlg)
	enum { IDD = IDD_COORD_DIALOG };
	double	m_Pitch;
	double	m_Roll;
	double	m_X;
	double	m_Y;
	double	m_Yaw;
	double	m_Z;
	int		m_Type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(coordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(coordDlg)
	virtual void OnCancel();
	afx_msg void OnKillfocusPitch();
	afx_msg void OnChangeRoll();
	afx_msg void OnKillfocusRoll();
	afx_msg void OnKillfocusX();
	afx_msg void OnKillfocusY();
	afx_msg void OnKillfocusYaw();
	afx_msg void OnKillfocusZ();
	virtual void OnOK();
	afx_msg void OnKillfocusType();
	afx_msg void OnF();
	afx_msg void OnM();
	afx_msg void OnV();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COORDDLG_H__D1FA294B_9C7C_11D5_BAE6_005004188CC7__INCLUDED_)
