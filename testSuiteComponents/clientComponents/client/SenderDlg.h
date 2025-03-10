// SenderDlg.h : header file
//

#if !defined(AFX_SENDERDLG_H__1170158F_BA51_11D4_BAD7_005004188CC7__INCLUDED_)
#define AFX_SENDERDLG_H__1170158F_BA51_11D4_BAD7_005004188CC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Helper macro for displaying errors
#define PRINTSTATUS(s)	\
		SetDlgItemText(IDC_STATUS, s)


#include <iostream.h>
#include <fstream.h>

// Timeout constants
const int ConnectTimerID = 10;
const int ReadTimerID = 20;
const int WriteTimerID = 30;
const int ConnectTimeOut = 10;  // Call every 10 milliseconds
const int ReadTimeOut = 10;		// Call every 10 milliseconds
const int WriteTimeOut = 750;	// Call every 1000 milliseconds

/////////////////////////////////////////////////////////////////////////////
// CSenderDlg dialog

class CSenderDlg : public CDialog
{
// Construction
public:
	CSenderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSenderDlg)
	enum { IDD = IDD_SENDER_DIALOG };
	CButton	m_Radio1;
	CListBox	m_MessageListBox;
	CString	m_LogFile;
	CString	m_Hostname;
	short	m_SocketNumber;
	CString	m_Status;
	CString	m_CMMDriverFile;
	CString	m_ManualCommand;
	//}}AFX_DATA

	// I added these variables
	fstream CMMDriverFile;
	fstream LogFile;
	fstream IniFile;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSenderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSenderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSetLogfile();
	afx_msg void OnExecute();
	afx_msg void OnSetPlonkavisionFile();
	afx_msg void OnKillfocusHostname();
	afx_msg void OnKillfocusSocketPortNumber();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnKillfocusStatus();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSetCmmdriverFile();
	afx_msg void OnDMISFile();
	afx_msg void OnCMMDriverFile();
	afx_msg void OnSingleStep();
	afx_msg void OnSetManualCommand();
	afx_msg void OnKillfocusManualCommand();
	afx_msg void OnConnect();
	afx_msg void OnCancel2();
	afx_msg void OnKillfocusMessageListBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SENDERDLG_H__1170158F_BA51_11D4_BAD7_005004188CC7__INCLUDED_)
