// RobotCoordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sender.h"
#include "CoordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// RobotCoordDlg dialog


CoordDlg::CoordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CoordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(RobotCoordDlg)
	m_Pitch = 0.0;
	m_Roll = 0.0;
	m_X = 0.0;
	m_Y = 0.0;
	m_Yaw = 0.0;
	m_Z = 0.0;
	//}}AFX_DATA_INIT
}


void CoordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CoordDlg)
	DDX_Text(pDX, IDC_PITCH, m_Pitch);
	DDX_Text(pDX, IDC_ROLL, m_Roll);
	DDX_Text(pDX, IDC_X, m_X);
	DDX_Text(pDX, IDC_Y, m_Y);
	DDX_Text(pDX, IDC_YAW, m_Yaw);
	DDX_Text(pDX, IDC_Z, m_Z);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CoordDlg, CDialog)
	//{{AFX_MSG_MAP(RobotCoordDlg)
	ON_EN_KILLFOCUS(IDC_PITCH, OnKillfocusPitch)
	ON_EN_KILLFOCUS(IDC_ROLL, OnKillfocusRoll)
	ON_EN_KILLFOCUS(IDC_X, OnKillfocusX)
	ON_EN_KILLFOCUS(IDC_Y, OnKillfocusY)
	ON_EN_KILLFOCUS(IDC_YAW, OnKillfocusYaw)
	ON_EN_KILLFOCUS(IDC_Z, OnKillfocusZ)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// RobotCoordDlg message handlers

void CoordDlg::OnKillfocusPitch() 
{
	// TODO: Add your control notification handler code here
	
}

void CoordDlg::OnKillfocusRoll() 
{
	// TODO: Add your control notification handler code here
	
}

void CoordDlg::OnKillfocusX() 
{
	// TODO: Add your control notification handler code here
	
}

void CoordDlg::OnKillfocusY() 
{
	// TODO: Add your control notification handler code here
	
}

void CoordDlg::OnKillfocusYaw() 
{
	// TODO: Add your control notification handler code here
	
}

void CoordDlg::OnKillfocusZ() 
{
	// TODO: Add your control notification handler code here
	
}

void CoordDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CoordDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
