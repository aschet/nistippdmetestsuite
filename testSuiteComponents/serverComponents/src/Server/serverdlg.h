// ServerDlg.h : header file
//


#if !defined(AFX_SERVERDLG_H__880456D6_BB2B_11D4_BAD7_005004188CC7__INCLUDED_)
#define AFX_SERVERDLG_H__880456D6_BB2B_11D4_BAD7_005004188CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "commonInterface3.h"

#include <iostream>
#include <fstream>

//*3 fstream LogFile;

/////////////////////////////////////////////////////////////////////////////
// CServerDlg dialog

class CServerDlg : public CDialog
{
// Construction
public:
	CServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CServerDlg)
	enum { IDD = IDD_SERVER_DIALOG };
	CListBox	m_MessageListBox;
	CString	m_LogFile;
	short	m_SocketNumber;
	CString	m_Status;
	short	m_ErrorCode;
	double  m_ProbeRadius;
	//}}AFX_DATA

	// I added these variables
	//*3 fstream LogFile;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetLogfile();
	afx_msg void OnExecute();
	afx_msg void OnCancel();
	afx_msg void OnKillfocusLogfile();
	afx_msg void OnKillfocusSocketPortNumber();
	virtual void OnOK();
	afx_msg void OnKillfocusStatus();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKillfocusErrorCode();
	afx_msg void OnNoLogParser();
	afx_msg void OnYesLogParser();
	afx_msg void OnSelchangeMessageListBox();
	afx_msg void On_Click_List(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeProbeRadius();
	afx_msg void OnChangeSetProbeRadius();
	afx_msg void OnChangeEdit1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDLG_H__880456D6_BB2B_11D4_BAD7_005004188CC7__INCLUDED_)
