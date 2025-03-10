// coordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sender.h"
#include "coordDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// coordDlg dialog

int input_type = NONE;

CString inText;

coordDlg::coordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(coordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(coordDlg)
	m_Pitch = 0.0;
	m_Roll = 0.0;
	m_X = 0.0;
	m_Y = 0.0;
	m_Yaw = 0.0;
	m_Z = 0.0;
	m_Type = INPUT;
	//}}AFX_DATA_INIT
}


void coordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(coordDlg)
	DDX_Text(pDX, IDC_PITCH, m_Pitch);
	DDX_Text(pDX, IDC_ROLL, m_Roll);
	DDX_Text(pDX, IDC_X, m_X);
	DDX_Text(pDX, IDC_Y, m_Y);
	DDX_Text(pDX, IDC_YAW, m_Yaw);
	DDX_Text(pDX, IDC_Z, m_Z);
	DDX_Text(pDX, IDC_TYPE, m_Type);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(coordDlg, CDialog)
	//{{AFX_MSG_MAP(coordDlg)
	ON_EN_KILLFOCUS(IDC_PITCH, OnKillfocusPitch)
	ON_EN_CHANGE(IDC_ROLL, OnChangeRoll)
	ON_EN_KILLFOCUS(IDC_ROLL, OnKillfocusRoll)
	ON_EN_KILLFOCUS(IDC_X, OnKillfocusX)
	ON_EN_KILLFOCUS(IDC_Y, OnKillfocusY)
	ON_EN_KILLFOCUS(IDC_YAW, OnKillfocusYaw)
	ON_EN_KILLFOCUS(IDC_Z, OnKillfocusZ)
	ON_EN_KILLFOCUS(IDC_TYPE, OnKillfocusType)
	ON_BN_CLICKED(IDC_RADIO1, OnF)
	ON_BN_CLICKED(IDC_RADIO2, OnM)
	ON_BN_CLICKED(IDC_RADIO3, OnV)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// coordDlg message handlers



void coordDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

void coordDlg::OnKillfocusPitch() 
{
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnChangeRoll() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnKillfocusRoll() 
{
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnKillfocusX() 
{
	CString inText;

	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_X,inText);
	sscanf(inText, "%lf", &m_X);
}

void coordDlg::OnKillfocusY() 
{
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnKillfocusYaw() 
{
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnKillfocusZ() 
{
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}


void coordDlg::OnKillfocusType() 
{
	// TODO: Add your control notification handler code here
	
}

void coordDlg::OnF() 
{
	// TODO: Add your control notification handler code here
	m_Type = FROM_FILE;
	SetDlgItemInt(IDC_TYPE, m_Type);
}

void coordDlg::OnM() 
{
	// TODO: Add your control notification handler code here
	m_Type = MEASUREMENT;
	SetDlgItemInt(IDC_TYPE, m_Type);
}

void coordDlg::OnV() 
{
	// TODO: Add your control notification handler code here
	m_Type = INPUT;
	SetDlgItemInt(IDC_TYPE, m_Type);
}
