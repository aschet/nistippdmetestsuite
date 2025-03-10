// SenderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Sender.h"
#include "SenderDlg.h"
#include "coordDlg.h"

// Additional header files
#include <stdio.h>
#include <time.h>
#include <winsock.h>

#include "../comm/socket.h"
#include "../strparse/cmdStringParse.h"
#include "../CmdResClasses/IppCmdRes.h"
#include "../respParser/parserRes.h"


// Header files for DMIS interpreter code
// Not implemented yet

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// For DMIS interpreter
#define EXIT 1

#define DMIS_FILE 1000
#define DME_FILE 2000
#define NO_FILE 3000
#define MAX_BUF_SIZE 65535

#define KILLTIMER 1
#define UNKNOWN 2

// Global variables
int c_id = 1;
int final_cid = 0;
int clear_error_cid = 0;
bool error_state = FALSE;
bool first_error = TRUE;
int end_line, num_find;
char end_of_session[20] = "";
char eof_marker[20] = "";
int read_c_id = 1;
char first_message[MAX_BUF_SIZE] ="";
char second_message[MAX_BUF_SIZE] = "";
char assemblyBuffer[MAX_BUF_SIZE] ="";
char responseBuffer[MAX_BUF_SIZE] = "";
int ret_vals[1000];
int out_vals[1000];
int messages = 1;
int file_type = NO_FILE;
int probe_to_cid = 0;
bool measure_transform = FALSE;
float part_transform[6];
bool tags_in_file = FALSE;
bool tag_in_line = FALSE;

char tag[5] = "";
char send_tag[5] = "";
char read_tag[5] = "";
char response_char;
char final_tag[5] = "";
char eof_tag[5] = "";
int single_step_start = 0;

char Event_char = 'E';
char Command_char = '0';
char Tag_type;

parserRes parser1;
Response * aResponse;
int code;
char errorMessage[100];

int status;

Socket mySocket;
bool file_open = FALSE;
bool logfile_open = FALSE;
bool file_running = FALSE;
bool client_connected = FALSE;
bool single_step_mode = FALSE;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSenderDlg dialog

CSenderDlg::CSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSenderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSenderDlg)
	m_LogFile = _T("client.log");
	m_Hostname = _T("127.0.0.1");
	m_SocketNumber = 1294;
	m_Status = _T("");
	m_CMMDriverFile = _T("");
	m_ManualCommand = _T("ClearAllErrors()");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSenderDlg)
	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
	DDX_Control(pDX, IDC_MESSAGE_LIST_BOX, m_MessageListBox);
	DDX_Text(pDX, IDC_LOGFILE, m_LogFile);
	DDX_Text(pDX, IDC_HOSTNAME, m_Hostname);
	DDX_Text(pDX, IDC_SOCKET_PORT_NUMBER, m_SocketNumber);
	DDX_Text(pDX, IDC_STATUS, m_Status);
	DDX_Text(pDX, IDC_CMMDRIVER_FILE, m_CMMDriverFile);
	DDX_Text(pDX, IDC_MANUAL_COMMAND, m_ManualCommand);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSenderDlg, CDialog)
	//{{AFX_MSG_MAP(CSenderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET_LOGFILE, OnSetLogfile)
	ON_BN_CLICKED(IDC_EXECUTE, OnExecute)
	ON_EN_KILLFOCUS(IDC_HOSTNAME, OnKillfocusHostname)
	ON_EN_KILLFOCUS(IDC_SOCKET_PORT_NUMBER, OnKillfocusSocketPortNumber)
	ON_EN_KILLFOCUS(IDC_STATUS, OnKillfocusStatus)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SET_CMMDRIVER_FILE, OnSetCmmdriverFile)
	ON_BN_CLICKED(IDC_RADIO2, OnDMISFile)
	ON_BN_CLICKED(IDC_RADIO1, OnCMMDriverFile)
	ON_BN_CLICKED(IDC_SINGLE_STEP, OnSingleStep)
	ON_BN_CLICKED(IDC_SET_MANUAL_COMMAND, OnSetManualCommand)
	ON_EN_KILLFOCUS(IDC_MANUAL_COMMAND, OnKillfocusManualCommand)
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDCANCEL2, OnCancel2)
	ON_LBN_KILLFOCUS(IDC_MESSAGE_LIST_BOX, OnKillfocusMessageListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSenderDlg message handlers

BOOL CSenderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	char buf[100];
	CString inText;

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_MessageListBox.SetHorizontalExtent(MAX_BUF_SIZE);

	// Open the .ini file for initialization
	IniFile.open("client.ini", std::ios::in);

	// Read the Hostname from the file
	IniFile.getline(buf, sizeof(buf));

	SetDlgItemText(IDC_HOSTNAME,buf);


	GetDlgItemText(IDC_HOSTNAME, inText);
	m_Hostname = inText.GetBuffer(1);

	SetDlgItemText(IDC_LOGFILE, m_LogFile);
	
	/// Default to I++ DME file
	m_Radio1.SetCheck(1);
	file_type = DME_FILE;

	// Close .ini file and open for writing
	IniFile.close();
	IniFile.open("client.ini", std::ios::out);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSenderDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSenderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/********************************************************/
/*                                                      */
/*       Set the logfile                                */
/*                                                      */
/********************************************************/

void CSenderDlg::OnSetLogfile() 
{
	// TODO: Add your control notification handler code here

	static char BASED_CODE szDefExt[] = "*.log";
	static char BASED_CODE szFileName[] = "client.log";
	static char BASED_CODE szFilter[] = "Log File (*.log)|*.log|All Files (*.*)|*.*||";

	CFileDialog pDlg(TRUE, szDefExt, szFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
	char** scriptFileName;

	if (logfile_open == TRUE)
	{
		// Close  current logfile
		LogFile.close();
		logfile_open = FALSE;
	}

	scriptFileName = &pDlg.m_ofn.lpstrFile;

	int nResponse = pDlg.DoModal();
	if (nResponse == IDOK)
	{
		m_LogFile = *scriptFileName;
		logfile_open = TRUE;
	}

	SetDlgItemText(IDC_LOGFILE, m_LogFile);
	
	LogFile.open(m_LogFile, std::ios::out);

}



void CSenderDlg::OnExecute() 
{
	// TODO: Add your control notification handler code here
	
	char linebuf[MAX_BUF_SIZE] = "";
	char timestr[20] = "";
	time_t t;
	t = time(NULL);
	char hostName[80] = "";
	char buf[MAX_BUF_SIZE] = "";

	single_step_mode = FALSE;

	// Reset the c_id
	c_id = 1;
	
	CString inText;

	if (file_type == NO_FILE)
	{
		PRINTSTATUS("Please select driver file type ...");
		return;
	}
	
	if (logfile_open == FALSE)
	{
		// Set the default logfile
		LogFile.open("client.log", std::ios::out);
		logfile_open = TRUE;

	}

	if (client_connected == FALSE)
	{
		PRINTSTATUS("Please connect to CMM server ...");
		//return;
	}


	else
	{
		// PRINTSTATUS("Running driver file...");
	

		file_running = TRUE;

		sprintf(buf, "*********** Starting DME file: %s **********\n", m_CMMDriverFile);
		writeLogFile(buf, LogFile);

		// Read line and send to server (timer loop)
		SetTimer(WriteTimerID, WriteTimeOut, 0);

	
		// Read response (non ACK) from the server (timer loop)
		SetTimer(ReadTimerID, ReadTimeOut, 0);
	}

	
}




void CSenderDlg::OnSetCmmdriverFile() 
{
	// TODO: Add your control notification handler code here
	
	char buf[100] = "";
	char** scriptFileName = NULL;
	char** transformFileName = NULL;

	if (file_type == NO_FILE)
	{
		SetDlgItemText(IDC_CMMDRIVER_FILE, "Please select driver file type");
	}

	if (file_type == DME_FILE && file_running == FALSE)
	{
		static char BASED_CODE szDefExt[] = "*.prg";
		static char BASED_CODE szFileName[] = "filename.prg";
		static char BASED_CODE szFilter[] = "I++ DME File (*.prg)|*.prg|All Files (*.*)|*.*||";
		CFileDialog pDlg(TRUE, szDefExt, szFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);

		scriptFileName = &pDlg.m_ofn.lpstrFile;

		int nResponse = pDlg.DoModal();
		if (nResponse == IDOK)
		{
			m_CMMDriverFile = *scriptFileName;
		}

		SetDlgItemText(IDC_CMMDRIVER_FILE, m_CMMDriverFile);

		if (file_open == TRUE)
		{
			CMMDriverFile.close();
			file_open = FALSE;
		}

		if (file_open == FALSE)
		{// Open the file for reading
		CMMDriverFile.open(m_CMMDriverFile, std::ios::in);
		file_open = TRUE;
		}

		// Clear out Socket Message ListBox
		m_MessageListBox.ResetContent();

		// Query for transform file
		// Or if there is no file, query for the 6 points to create
		// the transform
		// This is not implemented yet

		/*
		coordDlg cDlg;
		double tt;
		double *x,*y,*z,*rl,*pt,*yw;
		double x_val, y_val, z_val, r_val, p_val, yaw_val;
		int *input, input_val;

		x = &cDlg.m_X;
		y = &cDlg.m_Y;
		z = &cDlg.m_Z;
		rl = &cDlg.m_Roll;
		pt = &cDlg.m_Pitch;
		yw = &cDlg.m_Yaw;
		input = &cDlg.m_Type;

		

		nResponse = cDlg.DoModal();


		if (nResponse == IDOK)
		{

			//input = cDlg.m_Type;
			input_val = *input;

			if (input_val == INPUT)
			{
			// Get the values from the input fields
			tt = cDlg.m_X;
			x_val = *x;
			y_val = *y;
			z_val = *z;
			r_val = *rl;
			p_val = *pt;
			yaw_val = *yw;
		
			// Set the part transform
			part_transform[0] = x_val;
			part_transform[1] = y_val;
			part_transform[2] = z_val;
			part_transform[3] = r_val;
			part_transform[4] = p_val;
			part_transform[5] = yaw_val;

			}

			// If file set this

			else if (input_val == FROM_FILE)
			{

			sprintf(buf, "Type: %i\n", input_val);
			writeLogFile(buf, LogFile);

			static char BASED_CODE szDefExt[] = "*.trn";
			static char BASED_CODE szFileName[] = "filename.trn";
			static char BASED_CODE szFilter[] = "Part Transform File (*.trn)|*.trn|All Files (*.*)|*.*||";
			CFileDialog tDlg(TRUE, szDefExt, szFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);

			transformFileName = &tDlg.m_ofn.lpstrFile;

			int nResponse = tDlg.DoModal();
			if (nResponse == IDOK)
			{
			
				// Open the file and parse the numbers
				// Not implemented
				part_transform[0] = 0.0;
				part_transform[1] = 0.0;
				part_transform[2] = 0.0;
				part_transform[3] = 0.0;
				part_transform[4] = 0.0;
				part_transform[5] = 0.0;
			}
			}

			// If measurement set this

			else if (input_val == MEASUREMENT)
			{
			sprintf(buf, "Type: %i\n", input_val);
			writeLogFile(buf, LogFile);

			// Have to measure the transform -> set flag
			measure_transform = TRUE;

			}					
		}
		*/

	}
    
	if (file_type == DMIS_FILE)
	{
		// Not implemented
	
	}
	
}

void CSenderDlg::OnKillfocusHostname() 
{
	CString inText;
	GetDlgItemText(IDC_HOSTNAME, inText);
	m_Hostname = inText.GetBuffer(1);
	// TODO: Add your control notification handler code here
	
}

void CSenderDlg::OnKillfocusSocketPortNumber() 
{
	// TODO: Add your control notification handler code here
	
}

// Quitting the application

void CSenderDlg::OnCancel()
{
	// TODO: Add extra cleanup here


	// Write the Hostname to the file
	IniFile.write(m_Hostname, strlen(m_Hostname));
	
	// Close all files
	IniFile.close();
	CMMDriverFile.close();
	LogFile.close();

	file_open = FALSE;
	logfile_open = FALSE;

	//if (file_type == DMIS_FILE)
	//	interp_exit();

	// Close the socket
	mySocket.closeSocket();

	// Sleep loop
	for (int i=0; i<1000; i++)

	// Release WinSock
	WSACleanup();
	client_connected = FALSE;
	
	CDialog::OnCancel();
}

void CSenderDlg::OnOK() 
{
	// TODO: Add extra validation here
	// Have to comment out because of returns killing program
   	// CDialog::OnOK();
}

void CSenderDlg::OnKillfocusStatus() 
{
	// TODO: Add your control notification handler code here
	
}

void CSenderDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	char buf[MAX_BUF_SIZE] = "";
	char sendbuf[MAX_BUF_SIZE] = "";
	char linebuf[MAX_BUF_SIZE] = "";
	char replybuf[MAX_BUF_SIZE] = "";
	char word[MAX_BUF_SIZE] = "";
	char line [MAX_BUF_SIZE] = "";
	char new_line[MAX_BUF_SIZE] = "";
	char message[MAX_BUF_SIZE] = "";
	int nRet;
	int readin;
	int read_ack = 0;
	int command = 0;
	int id = 0;
	int cid = 0;
	char error_message[MAX_BUF_SIZE] = "";
	char data_message[MAX_BUF_SIZE] = "";

	bool responseReady = false;

	// Set the EOF marker (now it is a ::)
	sprintf(eof_marker, ":");

	// Set the End of Session marker
	sprintf(end_of_session, "EndSession()");

	memset (responseBuffer, 0, sizeof(responseBuffer));


	
/********************************************************/
/*                                                      */
/*      Timer for socket connect                        */
/*                                                      */
/********************************************************/

	if (nIDEvent == ConnectTimerID)
	{
	
		nRet = mySocket.connectSocket();

		if (nRet == WSAEWOULDBLOCK)
		{		
			PRINTSTATUS("Trying to connect ... ");	
		}
				
		else if (nRet == WSAEISCONN)
		{
			PRINTSTATUS ("Connected to server");
			KillTimer(ConnectTimerID);
			client_connected = TRUE;
		}
		
		else if (nRet != 0)
		{
			sprintf(buf, "Connect Error %d", nRet);
			PRINTSTATUS(buf);
			return;
		
		}

	}

	// Read the socket from the server
	else if (nIDEvent == ReadTimerID){

		memset (replybuf, 0, sizeof(replybuf));

		nRet = mySocket.readSocket(replybuf);


		if (nRet == WSAEWOULDBLOCK)
		{	
			// Nothing to read on the socket
			//PRINTSTATUS("Waiting for data ...");
		}


		else if (nRet != 0)
		{
			sprintf(buf, "Socket Error %d", nRet);
			PRINTSTATUS(buf);
			return;
		
		}

		else
		{
		
		/********************************************************/
		/*                                                      */
		/*      Read data from the socket                       */
		/*                                                      */
		/********************************************************/
		
		//	sprintf(buf, "Read from socket: %s", replybuf);
		//	m_MessageListBox.InsertString(0, buf);

		// Append the replybuf to assemblyBuffer
		strcat (assemblyBuffer, replybuf);

		}

		if (assemblyBuffer[0] != NULL)
		{
			int i = 0;
			int lgth = strlen(assemblyBuffer)-1;
			responseReady = false;

			// Joe fixed his original code here

			while (responseReady == false && i<lgth)
			{				
				// Look for a carraige-return line-feed
				if (assemblyBuffer[i] == 13 && assemblyBuffer[i+1] == 10)
					{
						int slicePoint = i+1;

						// Copy command to responseBuffer
						for(int j=0; j<=slicePoint; j++){
							responseBuffer[j]=assemblyBuffer[j];	
						}
					
						int l=0;

						// Remove command from assemblyBuffer and shift contents
						for(int k=slicePoint+1; k<=lgth; k++){
							assemblyBuffer[l++]=assemblyBuffer[k];
						}

						assemblyBuffer[l]=NULL;
						responseReady = true;

					} //end if

					i++;

			} // end while
			

			if(responseReady == true) {
			
				// Display the received data
			
				sprintf(buf, "Received: %s", responseBuffer);
				//	for (unsigned int i=0; i < strlen(buf); i++)
				//	{
				//		if (buf[i] == 13)
				//			buf[i] = '@';
				//		if (buf[i] == 10)
				//			buf[i] = '*';
				//	}
				PRINTSTATUS(buf);

				// Write to Socket Message ListBox
				m_MessageListBox.InsertString(0, buf);

				// Write response to logfile
				writeLogFile(buf, LogFile);

				// Joe implemented Tom's response parser here

				//*******************************************//
				//*                                         *//
				//*         Call Response Parser            *//
				//*                                         *//
				//*******************************************//

				parser1.setInput(responseBuffer);
				aResponse = parser1.parseResponse();
				code = parser1.getParserErr();				

				if (code != OK)
				{
					parser1.getErrorMessageString(errorMessage);
					sprintf(buf,"Response Error: %s", errorMessage);
					writeLogFile(buf, LogFile);
					m_MessageListBox.InsertString(0,buf);
				}

				// Copy the responseBuffer to message to parse
				strcpy (message, responseBuffer);

				// A "(" is used as the delimeter which occures after major word 
				//find_delimiter(' ', message, word, new_line);
      		
				response_char = message[6];
				sscanf(message, "%s", read_tag);
			

				// Check to see if this is an Event tag
				if (message[0] == 'E')
				{

					// Event Tag
					// If message is E0000 this is an unsolicited event
					// such as as Error from the server because an illegal
					// tag was sent

				}

				// Check for the ACK from previous command
				if (response_char == '&')
				{
			
					if (strcmp (tag, read_tag) == 0)
					{
						// Read the c_id from the previous command
						read_c_id = 1;
					
						c_id++;
				
					}
					else
					{
						PRINTSTATUS("Tags do not match");
					}
				}

				else if (response_char == '!')
				{

					if (strcmp (tag, read_tag) == 0)
					{

						// An Error was returned
					
						read_c_id = 1;
						//error_state = TRUE;
					
						// Get the error message
						for (int x = 8; (message[x] != '\0'); x++)
							error_message[x-8] = message[x];
				
						//writeLogFile(error_message, LogFile);
						c_id++;	

					}

					PRINTSTATUS("ERROR Returned");

					// Have to parse the error to check severity
					// Anything larger than 1 requires ClearAllErros be sent

					if (error_message[1] != '1')
					{
						PRINTSTATUS("ERROR - Must send ClearAllErrors()");
						KillTimer(WriteTimerID);
					}
					// Kill the ReadTimer
					// When the ack and done are back from the ClearAllErros
					// Can start the ReadTimer again
			
				}

				// Transaction complete 
				else if (response_char == '%')
				{
					sprintf(buf, "Read Done from Tag: %s", read_tag);
				

					if (strcmp (final_tag, read_tag) == 0)
					{
						sprintf(buf, "Session completed");
						PRINTSTATUS(buf);

						writeLogFile(buf, LogFile);

						memset (final_tag, 0, sizeof(final_tag));
				
					}

					if (strcmp(eof_tag, read_tag) == 0)
					{

						KillTimer(ReadTimerID);
						memset (eof_tag, 0, sizeof(eof_tag));

						PRINTSTATUS("Driver file completed");

						// Reset to the beginning of File
						CMMDriverFile.seekg(std::ios::beg);
						file_running = FALSE;

					}
				}

				// Data follows
				else if (response_char == '#')
				{

					for (int x = 8; (message[x] != '\0'); x++)
						data_message[x-8] = message[x];
				

				}


				// Bad value in response character
				else
				{
			
					sprintf(buf, "Unknown response character: %c", response_char);
					PRINTSTATUS (buf);
					writeLogFile(buf, LogFile);
				}
			}
		}
	//	} //end for assembly buffer

			//assemblyBuffer[0] = NULL;

	}
	else if ((nIDEvent == WriteTimerID) && (read_c_id == 1) && (error_state == FALSE)) {

	
		if(!CMMDriverFile.eof() && file_open == TRUE)

		{

			// File being used is a DME file
			if (file_type == DME_FILE)
			{
				// Read the next DME command
				CMMDriverFile.getline(linebuf, sizeof(linebuf));
				readin = CMMDriverFile.gcount();
			}

			// File being used is a DMIS program
			if (file_type == DMIS_FILE)
			{
				// Not implemented yet
			

			}


		if (strstr(linebuf, " StartSession") != NULL)
		{
			// Tags are in the file
			tags_in_file = TRUE;
		}

		if (strstr(linebuf, end_of_session) != NULL)
		{
		
			if (tags_in_file == TRUE)
				sscanf(linebuf, "%s", final_tag);

			else
			{
			
				final_cid = c_id;

				if (c_id < 10)
					sprintf (final_tag, "0000%d", final_cid);
				else if (c_id < 100)
					sprintf (final_tag, "000%d", final_cid);
				else if (c_id < 1000)
					sprintf (final_tag, "00%d", final_cid);
				else if (c_id < 10000)
					sprintf (final_tag, "0%d", final_cid);
				else
					sprintf (final_tag, "%d", final_cid);
			
			}

			sprintf(buf, "Final Tag: %s", final_tag);
			PRINTSTATUS (buf);
		

			// End of Session marker read
		//	sprintf(buf, "********** EndSession() read final_tag = %s **********", final_tag);
			writeLogFile(buf, LogFile);
		
			PRINTSTATUS("All lines of session sent");


		}

		// Check to see if this is the end of file or not
		if (strstr(linebuf, eof_marker) != NULL)	
		{

				strcpy (eof_tag, final_tag);

				// Close all files
				// CMMDriverFile.close();
				// file_open = FALSE;

				sprintf(buf, "Driver file completed");
				PRINTSTATUS(buf);

				// Reset to the beginning of File
				CMMDriverFile.seekg(std::ios::beg);
				file_running = FALSE;

				writeLogFile(buf, LogFile);
				//writeLogFile("\n\n", LogFile);

				memset (final_tag, 0, sizeof(final_tag));
		
		
				KillTimer(WriteTimerID);
			

		}
			


		// TODO: Add extra cleanup here

		if (status == EXIT && file_type == DMIS_FILE)

		{
			// Not implemented yet
			
		}

		else 
		{

			if (tags_in_file == FALSE)
			{
		
				if ((strstr(linebuf, "AbortE") == NULL) &&
					(strstr(linebuf, "GetPropE") == NULL) &&
					(strstr(linebuf, "OnMoveReportE") == NULL) &&
					(strstr(linebuf, "GetErrStatusE") == NULL))
				{
					Tag_type = '0';
				}

				else
					Tag_type = 'E';



				if (c_id < 10)
					sprintf (send_tag, "%c000%d", Tag_type, c_id);
				else if (c_id < 100)
					sprintf (send_tag, "%c00%d", Tag_type, c_id);
				else if (c_id < 1000)
					sprintf (send_tag, "%c0%d", Tag_type, c_id);
				else if (c_id < 10000)
					sprintf (send_tag, "%c%d", Tag_type, c_id);
				else
				{// c_id is too large
				}

			}

			else
				sscanf(linebuf, "%s", send_tag);

			// Copy the current tag
			strcpy(tag, send_tag);


			// Add a \r \n to the end of the line (in the spec)
			if  (tags_in_file == TRUE)
				sprintf (sendbuf, "%s\r\n", linebuf);
			else
				sprintf (sendbuf, "%s %s\r\n", send_tag, linebuf);

			
			// Do not write to socket if "\\" or ":"

			if ((strstr(linebuf, "\\") == NULL) &&
				(strcmp(linebuf, ":") != 0))

			{

				//Write line out socket
				nRet = mySocket.writeSocket(sendbuf);
	
				if (nRet != SOCKET_ERROR)
				{	
					// Write line to logfile
		
					sprintf(buf, "Sent: %s", sendbuf);
					PRINTSTATUS(buf);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, buf);

					writeLogFile(buf, LogFile);

					// Read the socket for the C_OK(c_id)
					// Must wait until this is received
					// Before sending new commands 

					read_c_id = 0;

				}  // End of if (SOCKET_ERROR)
				else
				{
					sprintf(buf, "Socket Error: %d", nRet);
					PRINTSTATUS(buf);
					client_connected = FALSE;
				}
					
			}
		}
	}  	// EOF so kill the Write Event
		
	else 
	{
			// Actual EOF (should never get here)
			// but if we do, kill the WriteTimer
			// KillTimer(WriteTimerID);

			sprintf(buf, "Hit the actual EOF");
			writeLogFile(buf, LogFile);
			CMMDriverFile.close();
			file_open = FALSE;
	}

	}

	else 
	{
		CDialog::OnTimer(nIDEvent);
	}

}

void CSenderDlg::OnDMISFile() 
{
	// TODO: Add your control notification handler code here
	// The program is running a DMIS file
	file_type = DMIS_FILE;
	
}

void CSenderDlg::OnCMMDriverFile() 
{
	// TODO: Add your control notification handler code here
	// The program is running a CMM driver file
	file_type = DME_FILE;
}


void CSenderDlg::OnSingleStep() 
{

	char buf[MAX_BUF_SIZE] = "";
	char sendbuf[MAX_BUF_SIZE] = "";
	char linebuf[MAX_BUF_SIZE] = "";
	char replybuf[MAX_BUF_SIZE] = "";
	char word[MAX_BUF_SIZE] = "";
	char line [MAX_BUF_SIZE] = "";
	char new_line[MAX_BUF_SIZE] = "";
	char message[MAX_BUF_SIZE] = "";
	int nRet;
	int readin;
	int read_ack = 0;
	int command = 0;
	int id = 0;
	int cid = 0;

	single_step_mode = TRUE;

	// TODO: Add your control notification handler code here

	// Read line and send to server (timer loop)

		if (client_connected == FALSE)
		{
			PRINTSTATUS("Please connect to server ...");
			return;
		}

		if (single_step_start == 0)
		{
			sprintf(buf, "*********** Starting DME file: %s **********", m_CMMDriverFile);
			writeLogFile(buf, LogFile);
			single_step_start = 1;
			file_running = TRUE;
		}

		//Joe added "&& (read_c_id == 1)" to wait for ack on single step
		//To "break" the Client, take this flag out to send multiple 
		//commands with no acks to Server

		if(!CMMDriverFile.eof() && (read_c_id == 1) && file_open == TRUE)

		{
			// File being used is a DME file
			if (file_type == DME_FILE)
			{
				// Read the next CMM driver command
				CMMDriverFile.getline(linebuf, sizeof(linebuf));
				readin = CMMDriverFile.gcount();
			}

			// File being used is a DMIS program
			if (file_type == DMIS_FILE)
			{
				// Not implemented yet
		
			}

			
		// Tom's comment line
		if (strstr(linebuf, "\\") != NULL)
		{
			// Read the next CMM driver command
			CMMDriverFile.getline(linebuf, sizeof(linebuf));
			readin = CMMDriverFile.gcount();
		}

		if (strstr(linebuf, " StartSession") != NULL)
		{
			// Tags are in the file
			tags_in_file = TRUE;
		}

		if (strstr(linebuf, end_of_session) != NULL)
		{
			

			if (tags_in_file == FALSE)
			{
			
				final_cid = c_id;

				if (c_id < 10)
					sprintf (final_tag, "0000%d", final_cid);
				else if (c_id < 100)
					sprintf (final_tag, "000%d", final_cid);
				else if (c_id < 1000)
					sprintf (final_tag, "00%d", final_cid);
				else if (c_id < 10000)
					sprintf (final_tag, "0%d", final_cid);
				else
					sprintf (final_tag, "%d", final_cid);

			}
			else
				sscanf(linebuf, "%s", final_tag);


			sprintf(buf, "Final Tag: %s", final_tag);
			PRINTSTATUS (buf);
		




		// End of Session marker read
		//	sprintf(buf, "********** EndSession() read final_tag = %s **********", final_tag);
			writeLogFile(buf, LogFile);
			PRINTSTATUS("All program lines of session sent");


		}

		// Check to see if this is the end of file or not
		if (strstr(linebuf, eof_marker) != NULL)	
		{

				CMMDriverFile.getline(linebuf, sizeof(linebuf));
				readin = CMMDriverFile.gcount();
				
				// Close all files

				//CMMDriverFile.close();
				single_step_start = 1;
				//file_open = FALSE;

				sprintf(buf, "Driver file completed");
				PRINTSTATUS(buf);

				// Reset to the beginning of File
				CMMDriverFile.seekg(std::ios::beg);
				file_running = FALSE;

				writeLogFile(buf, LogFile);

				memset (final_tag, 0, sizeof(final_tag));
		
				KillTimer(WriteTimerID);
					

		}
			


		// TODO: Add extra cleanup here

		if (status == EXIT && file_type == DMIS_FILE)

		{
			// Not implemented yet
			
		}

		else 
		{

			if (tags_in_file == FALSE)
			{
		
				if ((strstr(linebuf, "AbortE") == NULL) &&
					(strstr(linebuf, "GetPropE") == NULL) &&
					(strstr(linebuf, "OnMoveReportE") == NULL) &&
					(strstr(linebuf, "GetErrStatusE") == NULL))
				{
					Tag_type = '0';
				}

				else
					Tag_type = 'E';



				if (c_id < 10)
					sprintf (send_tag, "%c000%d", Tag_type, c_id);
				else if (c_id < 100)
					sprintf (send_tag, "%c00%d", Tag_type, c_id);
				else if (c_id < 1000)
					sprintf (send_tag, "%c0%d", Tag_type, c_id);
				else if (c_id < 10000)
					sprintf (send_tag, "%c%d", Tag_type, c_id);
				else
				{// c_id is too large
				}

			}

			else
				sscanf(linebuf, "%s", send_tag);

			// Copy the current tag
			strcpy(tag, send_tag);


			// Add a \r \n to the end of the line (in the spec)
			if  (tags_in_file == TRUE)
				sprintf (sendbuf, "%s\r\n", linebuf);
			else
				sprintf (sendbuf, "%s %s\r\n", send_tag, linebuf);

			
			// Do not write to socket if or ":"

			if ((strstr(linebuf, "\\") == NULL) &&
				(strcmp(linebuf, ":") != 0))

			{

				//Write line out socket
				nRet = mySocket.writeSocket(sendbuf);
	
				if (nRet != SOCKET_ERROR)
				{	
					// Write line to logfile
		
					sprintf(buf, "Sent: %s", sendbuf);
					PRINTSTATUS(buf);

						// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, buf);
					writeLogFile(buf, LogFile);

					// Read the socket for the C_OK(c_id)
					// Must wait until this is received
					// Before sending new commands 

					read_c_id = 0;

				}  // End of if (SOCKET_ERROR)
					
			}
		}

		// Read response (non ACK) from the server (timer loop)
		SetTimer(ReadTimerID, ReadTimeOut, 0);

	}  	// EOF so kill the Write Event
}


void CSenderDlg::OnSetManualCommand() 
{

	char buf[MAX_BUF_SIZE] = "";
	char sendbuf[MAX_BUF_SIZE] = "";
	char linebuf[MAX_BUF_SIZE] = "";
	char replybuf[MAX_BUF_SIZE] = "";
	char word[MAX_BUF_SIZE] = "";
	char line [MAX_BUF_SIZE] = "";
	char new_line[MAX_BUF_SIZE] = "";
	char message[MAX_BUF_SIZE] = "";
	int nRet;
	int read_ack = 0;
	int command = 0;
	int id = 0;
	int cid = 0;


	// TODO: Add your control notification handler code here
	// Read the command

	// Get the command out of the buffer

	if (client_connected == FALSE)
	{
		PRINTSTATUS("Please connect to server ...");
		return;
	}

	GetDlgItemText(IDC_MANUAL_COMMAND,m_ManualCommand);

	sprintf (linebuf, "%s", m_ManualCommand.GetBuffer(1));

	// Send the command to the controller

	
		
			if ((linebuf[0] == 'E' || linebuf[0] == '0') &&
				(linebuf[5] == ' '))
				tag_in_line = TRUE;
			else
				tag_in_line = FALSE;

			if (tag_in_line == TRUE)
				sscanf(linebuf, "%s", send_tag);

			else
			{
				if (c_id < 10)
					sprintf (send_tag, "0000%d", c_id);
				else if (c_id < 100)
					sprintf (send_tag, "000%d", c_id);
				else if (c_id < 1000)
					sprintf (send_tag, "00%d", c_id);
				else if (c_id < 10000)
					sprintf (send_tag, "0%d", c_id);
				else
					sprintf (send_tag, "%d", c_id);

			}

			// Copy the cuurent tag
			strcpy(tag, send_tag);

			if (strstr(linebuf, "ClearAllErrors()") != NULL)
			{
				if (single_step_mode == FALSE)
				{
					// Start the WriteTimer again
					SetTimer(WriteTimerID, WriteTimeOut, 0);
				}
			}

			if (strstr(linebuf, end_of_session) != NULL)
			{
		
			strcpy(final_tag, send_tag);

			
			final_cid = c_id;

			if (c_id < 10)
				sprintf (final_tag, "0000%d", final_cid);
			else if (c_id < 100)
				sprintf (final_tag, "000%d", final_cid);
			else if (c_id < 1000)
				sprintf (final_tag, "00%d", final_cid);
			else if (c_id < 10000)
				sprintf (final_tag, "0%d", final_cid);
			else
				sprintf (final_tag, "%d", final_cid);

			

			sprintf(buf, "Final Tag: %s", final_tag);
			PRINTSTATUS (buf);
			// writeLogFile(buf, LogFile);

			// EOF marker read so kill WriteTimer
			sprintf(buf, "EndSession() read final_cid = %d", final_cid);
			writeLogFile(buf, LogFile);
			PRINTSTATUS("All lines in session sent");

			}
		

			if (strstr(linebuf, eof_marker) != NULL)	
			{

			strcpy (eof_tag, final_tag);

			// Close all files
			//CMMDriverFile.close();
			//file_open = FALSE;

			sprintf(buf, "Driver file completed");
			PRINTSTATUS(buf);

			// Reset to the beginning of File
			CMMDriverFile.seekg(std::ios::beg);
			file_running = FALSE;

			writeLogFile(buf, LogFile);

			memset (final_tag, 0, sizeof(final_tag));
		
			KillTimer(WriteTimerID);
		
			return;

			}

			// Add a \r \n to the end of the line (in the spec)
			if  (tag_in_line == TRUE)
				sprintf (sendbuf, "%s\r\n", linebuf);
			else
				sprintf (sendbuf, "%s %s\r\n", send_tag, linebuf);


			//Write line out socket
			nRet = mySocket.writeSocket(sendbuf);
	
			if (nRet != SOCKET_ERROR)
			{	
				// Write line to logfile
		
				sprintf(buf, "Sent: %s", sendbuf);
				PRINTSTATUS(buf);

					// Write to Socket Message ListBox
				m_MessageListBox.InsertString(0, buf);
				writeLogFile(buf, LogFile);

				// Read the socket for the C_OK(c_id)
				// Must wait until this is received
				// Before sending new commands

				read_c_id = 0;

			}  // End of if (SOCKET_ERROR)

		
			SetTimer(ReadTimerID, ReadTimeOut, 0);
		



}

void CSenderDlg::OnKillfocusManualCommand() 
{
	// TODO: Add your control notification handler code here
	
}

void CSenderDlg::OnConnect() 
{

	
	char hostName[80] = "";
	char buf[80] = "";
	CString inText;
	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;

	// Check to see if the 

	nRet = mySocket.connectSocket();

	if (nRet == WSAEISCONN)
	{		
		PRINTSTATUS("Already connected to CMM Server");	
	}
				
	else 
	{

	// Set variables
	GetDlgItemText(IDC_HOSTNAME,m_Hostname);


	GetDlgItemText(IDC_SOCKET_PORT_NUMBER, inText);
	sscanf(inText, "%i", &m_SocketNumber);
	
	PRINTSTATUS("Client connecting to server");

	// Initialize WinSock and check the version
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		PRINTSTATUS("Wrong version Winsock");
		return;
	
	}


	// Initialize the Socket instance
	// Hostname
	nRet = mySocket.setHostName(m_Hostname.GetBuffer(1));
	if (nRet == 1)
    {
        PRINTSTATUS("gethostbyname() Error");
		exit(-1);
       
    }

	// Port Number
	mySocket.setPortNumber(m_SocketNumber);
	
	
	// Create a TCP/IP stream socket
	nRet = mySocket.createSocket();
	if (nRet == INVALID_SOCKET)
	{
		PRINTSTATUS("Create socket() Error");
		return;
	}

	
	// Set the socket to non-blocking
	nRet = mySocket.setNonBlocking();
	if (nRet != 0)
	{
		sprintf(buf, "Non-blocking Error %d", nRet);
		PRINTSTATUS(buf);
		return;

	}
	
	// Try to connect to the server (timer loop)
	SetTimer(ConnectTimerID, ConnectTimeOut, 0);


	}
	
}


//  Disconnect from server, but don't quit application
void CSenderDlg::OnCancel2() 
{
	// TODO: Add your control notification handler code here
		
	// Close the socket
	mySocket.closeSocket();

	// Sleep loop
	for (int i=0; i<1000; i++)

	// Release WinSock
	WSACleanup();

	PRINTSTATUS("Disconnected from CMM server");	
	client_connected = FALSE;
}

void CSenderDlg::OnKillfocusMessageListBox() 
{
	// TODO: Add your control notification handler code here
	
}
