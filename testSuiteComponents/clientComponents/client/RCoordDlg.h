#if !defined(AFX_ROBOTCOORDDLG_H__51AA9317_BF67_11D1_A427_0020AFA361FC__INCLUDED_)
#define AFX_ROBOTCOORDDLG_H__51AA9317_BF67_11D1_A427_0020AFA361FC__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RobotCoordDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// RobotCoordDlg dialog

class CoordDlg : public CDialog
{
// Construction
public:
	CoordDlg(CWnd* pParent = NULL);   // standard constructor

	CEdit*	m_PitchCtrl;
	CEdit*	m_RollCtrl;
	CEdit*	m_XCtrl;
	CEdit*	m_YCtrl;
	CEdit*	m_YawCtrl;
	CEdit*	m_ZCtrl;

// Dialog Data
	//{{AFX_DATA(CoordDlg)
	enum { IDD = IDD_COORD };
	double	m_Pitch;
	double	m_Roll;
	double	m_X;
	double	m_Y;
	double	m_Yaw;
	double	m_Z;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CoordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CoordDlg)
	afx_msg void OnKillfocusPitch();
	afx_msg void OnKillfocusRoll();
	afx_msg void OnKillfocusX();
	afx_msg void OnKillfocusY();
	afx_msg void OnKillfocusYaw();
	afx_msg void OnKillfocusZ();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOTCOORDDLG_H__51AA9317_BF67_11D1_A427_0020AFA361FC__INCLUDED_)
