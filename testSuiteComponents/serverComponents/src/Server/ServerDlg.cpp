// ServerDlg.cpp : implementation file
//

#include "../Server/StdAfx.h"
#include "Server.h" 
#include "ServerDlg.h"  
#include "Logger.h"
#include "../CmdResClasses/IppCmdRes.h"
#include "../CmdParser/parserCmd.h"
#include "../CmdContextChecker/checkerCmd.h"
#include "../CMM/world.h"
#include "ServerQueues.h"
#include "../Executor/executor.h"
//#include <fstream> 
//#include <iostream.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// I included these header files
//*2 #include <stdio.h>
//*2 #include <time.h>

// Helper macro for displaying status
#define PRINTSTATUS(s)	\
		SetDlgItemText(IDC_STATUS, s);  //011901-1, uncommented *2

//std::fstream LogFile; //*3
using namespace std;

bool executing = false;


Logger serverLog;
parserCmd parser1;
parserCmd parser2;
checkerCmd checker1;
world world1;
ServerQueues * serverQues= new ServerQueues();
Executor * executor1= new Executor(world1);
vector <Response *> responseList;




int timercount=0;


// Timeout stuff
const int ReadTimerID = 10;
const int ConnectTimerID = 30;
const int ExecuteTimerID = 40;

// To test Client for multi commands /w no ack use ReadInterval = 2000
const int ReadInterval = 20;  
const int ConnectInterval = 500;
const int ExecuteInterval = 50; 

SOCKET	listenSocket;
SOCKET	remoteSocket;

char assemblyBuffer[IPPSIZE] = "";
bool logOpen = false;

double probe_radius = 0.0;

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
// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CServerDlg)
	m_LogFile = _T("");
	m_SocketNumber = 1294;
	m_Status = _T("");
	m_ProbeRadius = 0.0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/*4:
	cmdQue = NULL;
	simStatus = simDone;
	probePosX = probePosY = probePosZ = 0.0;
	iUnit = jUnit = kUnit = 0.0;
	//LogFile.write("through constructor", 19);
	*/ //*4end, suspect not working
//	preCycleInits(); //031901
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CServerDlg)
	DDX_Control(pDX, IDC_MESSAGE_LIST_BOX, m_MessageListBox);
	DDX_Text(pDX, IDC_LOGFILE, m_LogFile);
	DDX_Text(pDX, IDC_SOCKET_PORT_NUMBER, m_SocketNumber);
	DDX_Text(pDX, IDC_STATUS, m_Status);
	DDX_Text(pDX, IDC_PROBE_RADIUS, m_ProbeRadius);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	//{{AFX_MSG_MAP(CServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SET_LOGFILE, OnSetLogfile)
	ON_BN_CLICKED(IDC_EXECUTE, OnExecute)
	ON_EN_KILLFOCUS(IDC_LOGFILE, OnKillfocusLogfile)
	ON_EN_KILLFOCUS(IDC_SOCKET_PORT_NUMBER, OnKillfocusSocketPortNumber)
	ON_EN_KILLFOCUS(IDC_STATUS, OnKillfocusStatus)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_NO_LOG_PARSER, OnNoLogParser)
	ON_BN_CLICKED(IDC_YES_LOG_PARSER, OnYesLogParser)
	ON_LBN_SELCHANGE(IDC_MESSAGE_LIST_BOX, OnSelchangeMessageListBox)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, On_Click_List)
	ON_EN_KILLFOCUS(IDC_PROBE_RADIUS, OnChangeSetProbeRadius)
	ON_BN_CLICKED(IDCANCEL2, OnCancel)
	//}}AFX_MSG_MAP
	//ON_EN_CHANGE
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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

	//******************************************
	//           Initializations Go Here
	//******************************************

	
	// initialize message box
	m_MessageListBox.SetHorizontalExtent(5000);


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CServerDlg::OnPaint() 
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CServerDlg::OnSetLogfile() 
{
	// TODO: Add your control notification handler code here

	static char BASED_CODE szDefExt[] = "*.log";
	static char BASED_CODE szFileName[] = "server.log";
	static char BASED_CODE szFilter[] = "Log File (*.log)|*.log|All Files (*.*)|*.*||";

	if(logOpen==true && world1.getInSession()==false){
		serverLog.closeFile();
		logOpen = false;
		PRINTSTATUS("Logfile Closed");
	} //end if
	else if(logOpen==true || world1.getInSession()==true){
		PRINTSTATUS("Logfile Busy");
	} // end else if

	if(logOpen==false && world1.getInSession()==false){

	// Clear out Socket Message ListBox
	m_MessageListBox.ResetContent();

	CFileDialog pDlg(TRUE, szDefExt, szFileName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);
	char** scriptFileName;

	scriptFileName = &pDlg.m_ofn.lpstrFile;

	int nResponse = pDlg.DoModal();
	if (nResponse == IDOK)
	{
		m_LogFile = *scriptFileName;
	

	SetDlgItemText(IDC_LOGFILE, m_LogFile);
	
	//LogFile.open(m_LogFile, ios::out);

	serverLog.openFile(m_LogFile);


	logOpen = true;
	PRINTSTATUS("Logfile Open");
	}
	}
}

void CServerDlg::OnExecute() 
{
	// TODO: Add your control notification handler code here
	if(executing){
		PRINTSTATUS("Already Executing")
	}

	else
	{

	executing = true;
	char szBuf[IPPSIZE] = "";


	WORD wVersionRequested = MAKEWORD(1,1);
	WSADATA wsaData;
	int nRet;

	//
	// Initialize WinSock and check the version
	//
	nRet = WSAStartup(wVersionRequested, &wsaData);
	if (wsaData.wVersion != wVersionRequested)
	{	
		PRINTSTATUS("Wrong version Winsock");
		return;
	
	}

	// Set variables
	char sss[100]="";
	CString inText;
	GetDlgItemText(IDC_SOCKET_PORT_NUMBER, inText);
	sscanf(inText, "%i", &m_SocketNumber);


	//** now a command to set the radius
	GetDlgItemText(IDC_PROBE_RADIUS, inText);
	sscanf(inText, "%lf", &probe_radius);
	//world1.setProbeRadius(probe_radius);
	

	//
	// Create a TCP/IP stream socket to "listen" with
	//
	listenSocket = socket(AF_INET,			// Address family
						  SOCK_STREAM,		// Socket type
						  IPPROTO_TCP);		// Protocol
	if (listenSocket == INVALID_SOCKET)
	{
		PRINTSTATUS("Create socket() Error");
		return;
	}

	//
	// Fill in the address structure
	//
	SOCKADDR_IN saServer;		

	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY;	// Let WinSock supply address
	saServer.sin_port = htons(m_SocketNumber); // Use port from GUI

	//
	// bind the name to the socket
	//

	nRet = bind(listenSocket,				// Socket 
				(LPSOCKADDR)&saServer,		// Our address
				sizeof(struct sockaddr));	// Size of address structure
	
	if (nRet == SOCKET_ERROR)
	{
		PRINTSTATUS("bind() Error");
		closesocket(listenSocket);
		return;
	}
	
	int nLen;
	nLen = sizeof(SOCKADDR);


	nRet = gethostname(szBuf, sizeof(szBuf));
	if (nRet == SOCKET_ERROR)
	{
		PRINTSTATUS("gethostname() Error");
		closesocket(listenSocket);
		return;
	}

	//
	// Set the socket to listen
	//

	nRet = listen(listenSocket,					// Bound socket
				  SOMAXCONN);					// Number of connection request queue
	if (nRet == SOCKET_ERROR)
	{
		PRINTSTATUS("listen() Error");
		closesocket(listenSocket);
		return;
	}
	

	// Set socket to non-blocking
	unsigned long arg = 1;
	if (ioctlsocket(listenSocket, FIONBIO, &arg)<0)
	{
		PRINTSTATUS("Couldn't set to non-blocking");
		return;
	}


	//
	// Initialize the parsers
	//
	//parser1.initParser();
	//parser2.initParser();
	//checker1.initChecker();
	//
	// Start the waith for a connection timmer
	//
	SetTimer(ConnectTimerID, ConnectInterval, 0);

	}
}

void CServerDlg::OnCancel() 
{
	// TODO: Add your control notification handler code here
			
	//
	// Connection closed by client
	// Close BOTH sockets before exiting
	//
	closesocket(remoteSocket);
	closesocket(listenSocket);

	//
	// Close logfile
	//
	serverLog.closeFile();

	//
	// Shut Winsock back down
	//
	WSACleanup();

	CDialog::OnCancel();
}

void CServerDlg::OnKillfocusLogfile() 
{
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::OnKillfocusSocketPortNumber() 
{
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	// Have to comment this out
	//CDialog::OnOK();
}



void CServerDlg::OnKillfocusStatus() 
{
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	
	char szBuf[IPPSIZE] = "";
	char cmdStoreBuff[IPPSIZE] = "";
	char statusBuffer[IPPSIZE] = "";
	char msgBuffer[IPPSIZE] = "";
	char segmentBuffer[IPPSIZE] = "";
	char commandBuffer[IPPSIZE] = "";
	char responseBuffer[IPPSIZE] = "";
	char responseBuffer2[IPPSIZE] = "";
	char ackBuffer[IPPSIZE] = "";
	char executeBuffer[IPPSIZE]="";
	char parseBuffer[IPPSIZE]="";
	char logBuffer[IPPSIZE]="";
	char errorMessage[100]="";

	int nRet;
	int parser1_code;
	int parser1_tag;
	int parser2_code;
	int checker1_code;

	bool disconnect = false;
	bool commandReady = false;

	long tmpT = 0;


	/*******************************************************************************/
	/*                                                                             */
	/*   Read from the socket connection every "ReadInterval" milliseconds         */
	/*                                                                             */
	/*******************************************************************************/

	if (nIDEvent == ReadTimerID)
	{
		memset(statusBuffer, 0, sizeof(statusBuffer));
		memset(msgBuffer, 0, sizeof(msgBuffer));
		memset(cmdStoreBuff, 0, sizeof(cmdStoreBuff));
		memset(segmentBuffer, 0, sizeof(segmentBuffer));
		memset(commandBuffer, 0, sizeof(commandBuffer));
		memset(responseBuffer, 0, sizeof(responseBuffer));
		memset(responseBuffer2, 0, sizeof(responseBuffer2));
		memset(parseBuffer, 0, sizeof(parseBuffer));
		memset(logBuffer, 0, sizeof(logBuffer));
		memset(errorMessage, 0, sizeof(errorMessage));
		
		nRet = recv(remoteSocket,				// Connected client
					commandBuffer,				// Receive buffer
					sizeof(commandBuffer),		// Length of buffer
					0);							// Flags
		
		/************************************************************/
		/*                                                          */
		/*       If there a socket disconnect:                      */
		/*          1. Notify the user                              */
		/*          2. Kill the read thread                         */
		/*          3. Kill the execute thread                      */
		/*          4. Start the connection thread                  */
		/*          5. Set a flag                                   */
		/*                                                          */
		/************************************************************/

		if ((nRet == SOCKET_ERROR && WSAGetLastError() != WSAEWOULDBLOCK) || nRet == 0)
		{
			PRINTSTATUS("Connection Closed by Client");
		
			//Kill the executor
			KillTimer(ExecuteTimerID);

			// Reset Executor
			executor1->resetExecutor();			

			// ClearIncommingBuffer and CommandBuffer
			memset(assemblyBuffer, 0, sizeof(assemblyBuffer));
			assemblyBuffer[0]=NULL;
			memset(commandBuffer, 0, sizeof(commandBuffer));

			// Clear All Queues
			serverQues->clearAllQueues();

			// Start listening for a new client
			SetTimer(ConnectTimerID, ConnectInterval, 0);
			disconnect = true;
			
			// Kill the reader/writer	
			KillTimer(ReadTimerID);

			//return;return;
						
		}

		/************ Nothing on socket to read *************/
		else if(WSAGetLastError() == WSAEWOULDBLOCK){ 
		
			
		
		}


		/************************************************************/
		/*                                                          */
		/*       Else, start read any data                          */
		/*       that's available at the socket                     */
		/*                                                          */
		/************************************************************/

		else 
		{ 

			// Append segmentBuffer to assemblyBuffer
			strcat(assemblyBuffer, commandBuffer);
		}

		if(assemblyBuffer[0] != NULL && world1.getReadiness() != aborted && disconnect == false)
		{
			//Scan the buffer for carriage returns and line feeds
			
			int crCount=0; 
			int lfCount=0;
			int lastChar = strlen(assemblyBuffer)-1;

			for (int i=0; i<=lastChar; i++)
			{
				if(assemblyBuffer[i]==13) { crCount++; }
				if(assemblyBuffer[i]==10) { lfCount++; }
			}	
			
		
			if (assemblyBuffer[lastChar-1]==13 && assemblyBuffer[lastChar]==10 &&
				crCount==1 && lfCount==1)
			{
				// Single command is ready	
				for (int i=0; i<=lastChar; i++)
					commandBuffer[i]=assemblyBuffer[i];
				for(i=0; i<=lastChar; i++)
					cmdStoreBuff[i]=assemblyBuffer[i];
			
				assemblyBuffer[0]=NULL;
				commandReady = true;
			}
			else if(crCount >= 2)
			{
				// Bad command protocol

				int j=0;
				for(i=0; i<=lastChar; i++)
				{
					if(assemblyBuffer[i]!=13 && assemblyBuffer[i]!=10){
					  cmdStoreBuff[j]=assemblyBuffer[i];
					  j++;
					}
					if((assemblyBuffer[i]==13 && i==lastChar-1) ||
						(assemblyBuffer[i]==10 && i==lastChar)){
					  cmdStoreBuff[j]=assemblyBuffer[i];
					  j++;
					}
				}
				

				assemblyBuffer[0]=NULL;
				commandReady = false;

				// Generate Error Response
				char errorCauseMethod[IPPSIZE/2]="";
				serverLog.replaceQuotes(cmdStoreBuff);
				sprintf(errorCauseMethod,"%s",cmdStoreBuff);
				memset(responseBuffer, 0, sizeof(responseBuffer));
				ErrorResponse *_ErrorResponse;
				_ErrorResponse = new ErrorResponse(0000, EventTag,
						                           ProtocolError, errorCauseMethod);
				_ErrorResponse->getResponseString(responseBuffer,IPPSIZE-1);

				if (_ErrorResponse->getTheError()->getSeverity() > '1'){
					world1.setReadiness(erred);
					serverQues->clearAllQueues();
				}
				//serverQues->setNeedToClearAllErrors(true);
				delete _ErrorResponse;

				//Log Ipp error response from parser
				memset(logBuffer, 0, sizeof(logBuffer));
				sprintf(logBuffer, "Sent: %s", responseBuffer);
				serverLog.writeFile(logBuffer);

				// Write to Socket Message ListBox
				m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

				// If set by user, write detailed parser errors to the logfile.
				if(serverLog.getLogDetails()==true){
					memset(logBuffer, 0, sizeof(logBuffer));
					memset(errorMessage, 0, sizeof(errorMessage));
					parser1.getErrorMessageString(errorMessage);
					sprintf(logBuffer, "Parser: %s\n", errorMessage);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());
				}

				// send tag syntax error response buffer to the world response que
				while(!serverQues->enqResp(serverQues->getRespKey(), strdup(responseBuffer))){} // Send to response que
			
				commandReady=false;

			}
			else 
			{
				// Only partial command transfered on socket
				commandReady = false;
			}
		
		/*	int i = 0;
			int lgth = strlen(assemblyBuffer)-1;
			commandReady = false;

			// Check assemblyBuffer for assembled command
			while (commandReady == false && i<lgth)
			{
				// Look for a carraige-return line-feed
				if (assemblyBuffer[i]==13 && assemblyBuffer[i+1]==10)
				{
					int slicePoint = i+1;

					// Copy command to commandBuffer
					for(int j=0; j<=slicePoint; j++){
						commandBuffer[j]=assemblyBuffer[j];	
					}

					// A copy of the command without the 13 & 10
					for(j=0; j<=slicePoint-2; j++){
						cmdStoreBuff[j]=assemblyBuffer[j];
					}

					int l=0;

					// Remove command from assemblyBuffer and shift contents
					for(int k=slicePoint+1; k<=lgth; k++){
						assemblyBuffer[l++]=assemblyBuffer[k];
					}

					assemblyBuffer[l]=NULL;
					commandReady = true;

				} // end if

				i++;

			} // end while*/
	
			if(commandReady == true) // A command including /r/n is available in command Buffer
			{
				// Log incomming command string to file
				sprintf(logBuffer,"received: %s",commandBuffer);
				serverLog.writeFile(logBuffer);

				// Write to Socket Message ListBox
				m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

				PRINTSTATUS(logBuffer);		

				parser1.setInput(commandBuffer); // Set the parser input string	 
				parser1.parseTag();				 // Parse the tag

				parser1_tag = parser1.getTag();
				if(parser1.getIsEvent()==1) parser1_tag = parser1_tag * (-1);

				tagIdType tt;

				if(parser1.getIsEvent()==1) tt=EventTag; // Determing if Event or Command Tag
				else tt=CommandTag;
			
				parser1_code = parser1.getParserErr();

				// If parser error (ie. bad tag), produce error Response
				if(parser1_code != OK)
				{
					char errorCauseMethod[IPPSIZE/2]="";
					serverLog.replaceQuotes(cmdStoreBuff);
					if(serverLog.getLogDetails()==true) {
						parser1.getErrorMessageString(errorMessage);
						sprintf(errorCauseMethod,"%s : %s",cmdStoreBuff, errorMessage);
					}
					else
						sprintf(errorCauseMethod,"%s",cmdStoreBuff);
					memset(responseBuffer, 0, sizeof(responseBuffer));
					ErrorResponse *_ErrorResponse;
					_ErrorResponse = new ErrorResponse(0000, EventTag,
						                                IllegalTag, errorCauseMethod);
					_ErrorResponse->getResponseString(responseBuffer,IPPSIZE-1);

					if (_ErrorResponse->getTheError()->getSeverity() > '1'){
						world1.setReadiness(erred);
						serverQues->clearAllQueues();
					}
						//serverQues->setNeedToClearAllErrors(true);
					delete _ErrorResponse;

					//Log Ipp error response from parser
					memset(logBuffer, 0, sizeof(logBuffer));
					sprintf(logBuffer, "Sent: %s", responseBuffer);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

					// If set by user, write detailed parser errors to the logfile.
					if(serverLog.getLogDetails()==true){
						memset(logBuffer, 0, sizeof(logBuffer));
						memset(errorMessage, 0, sizeof(errorMessage));
						parser1.getErrorMessageString(errorMessage);
						sprintf(logBuffer, "Parser: %s\n", errorMessage);
						serverLog.writeFile(logBuffer);

						// Write to Socket Message ListBox
						m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());
					}

					// send tag syntax error response buffer to the world response que
					while(!serverQues->enqResp(serverQues->getRespKey(), strdup(responseBuffer))){} // Send to response que
					
					commandReady=false;

				} // end check tag syntax	
			
					// If tag in session, produce an IllegalTag Response

				
				else if(serverQues->tagInList(parser1_tag) == true)  // Check the tag in session list
				{
					char errorCauseMethod[IPPSIZE/2]="";
					serverLog.replaceQuotes(cmdStoreBuff);
					if(serverLog.getLogDetails()==true)
						sprintf(errorCauseMethod,"%s : TAG NUMBER IN USE",cmdStoreBuff);
					else
						sprintf(errorCauseMethod,"%s",cmdStoreBuff);

					ErrorResponse *_ErrorResponse;
					_ErrorResponse = new ErrorResponse(0000, EventTag, IllegalTag, errorCauseMethod);
					_ErrorResponse->getResponseString(responseBuffer,IPPSIZE-1);
					if (_ErrorResponse->getTheError()->getSeverity() > '1'){
						world1.setReadiness(erred);
						serverQues->clearAllQueues();
					}
					delete _ErrorResponse;

					//Log Ipp error response
					memset(logBuffer, 0, sizeof(logBuffer));
					sprintf(logBuffer, "Sent: %s", responseBuffer);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

					// send tag in session error response to the world response que
					while(!serverQues->enqResp(serverQues->getRespKey(), strdup(responseBuffer))){} // Send to response que
					
					commandReady=false;

				} // end check tag in use

				else // It's a valid tag that's not in use
				{   
					
					serverQues->addTag(parser1_tag);         // Add tag # to active tag list
					
					/****************************************************************/
					/*                                                              */
					/*    If Command tag, place command on "slow queue"             */
					/*                                                              */
					/*    Else If Event tag, place command on "fast" queue          */
					/*                                                              */
					/****************************************************************/

					if (tt==CommandTag) 		    
					{
						
						while(!serverQues->enqSlow(serverQues->getSlowKey(), 
							strdup(commandBuffer))){} 						

						//memset(logBuffer, 0, sizeof(logBuffer));
						//sprintf(logBuffer,"Push Slow Queue: %s",commandBuffer);
						//serverLog.writeFile(logBuffer);
					}
					else if (tt==EventTag)
					{						
						while(!serverQues->enqFast(serverQues->getFastKey(), 
							strdup(commandBuffer))){} // put on fast queue
						//memset(logBuffer, 0, sizeof(logBuffer));
						//sprintf(logBuffer,"Push Fast Queue: %s",commandBuffer);
						//serverLog.writeFile(logBuffer);
					}

					// Build Ack Response
					Response *_Response;
					_Response = new Response(parser1.getTag(), tt, Ack);
					_Response->getResponseString(responseBuffer,IPPSIZE-1);
					delete _Response;

					//Log Ack Response
					memset(logBuffer, 0, sizeof(logBuffer));
					sprintf(logBuffer, "Sent: %s", responseBuffer);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

					// send response buffer to the world response que
					while(!serverQues->enqResp(serverQues->getRespKey(), strdup(responseBuffer))){} // Send to response que
					commandReady=false;

				}  // end valid tag		

			}// end if(commandReady)

		} // end process command
		
		if(disconnect==false){

			while(serverQues->respQueHasData()){

				char* resp;
				memset(responseBuffer, 0, sizeof(responseBuffer));
				while((resp = serverQues->deqResp(serverQues->getRespKey())) == NULL){} //get msg from response que
				sprintf(responseBuffer,"%s",resp);

				nRet = send(remoteSocket,          // Connected socket
						responseBuffer,	           // Data buffer
						strlen(responseBuffer),    // Length of data
						0);	                       // Flags

				PRINTSTATUS(responseBuffer);
			} // end while

		} // end if disconnect
	}
	
	/*******************************************************************************/
	/*                                                                             */
	/*   Process and execute command strings from the fast and slow queues         */
	/*   every "ReadInterval" milliseconds                                         */
	/*                                                                             */
	/*******************************************************************************/
	
	if (nIDEvent == ExecuteTimerID)
	{
		
		memset(executeBuffer, 0, sizeof(executeBuffer));
		char* cmd;
		serverQues->setCommandExistence(false);


		/*******************************************************************************/
		/*                                                                             */
		/*                          Check for data on fast queue first                 */
		/*                                                                             */
		/*******************************************************************************/
		
		if(serverQues->fastQueHasData()) 
		{	
			serverQues->setCommandExistence(true);
			while((cmd = serverQues->deqFast(serverQues->getFastKey())) == NULL){} //get cmd from fast que
			parser2.setInput(cmd);      // feed the cmd to the parser
		}

		
		/*******************************************************************************/
		/*                                                                             */
		/*      If no fast queue data and there is no multicycle command currently     */
		/*      running then check for data on the slow queue                          */
		/*                                                                             */
		/*******************************************************************************/

		//else if(serverQues->slowQueHasData() && executor1->checkMultiCycleCommand()==false)
		else if(serverQues->slowQueHasData() && world1.getReadiness() != moving)
		{	
			serverQues->setCommandExistence(true);
			while((cmd = serverQues->deqSlow(serverQues->getSlowKey())) == NULL){} //get cmd from fast que
			parser2.setInput(cmd);     // feed the cmd to the parser
		}

		/********************************************/
		/*                                          */
		/*  if a command was successfuly dequeued   */
		/*                                          */
		/********************************************/

		if(serverQues->getCommandExistence()) {
			parser2.parseTag();		// parse the tag
			//Check and record if event or command
			tagIdType tt;
			if(parser2.getIsEvent()==1) tt=EventTag;
			else tt=CommandTag;
			
			Command* aCommand = parser2.parseCommand();    // parse the command
			parser2_code = parser2.getParserErr();
			memset(executeBuffer, 0, sizeof(executeBuffer));
			if (parser2_code == OK){						// If the command parses OK

				//**********  clear errors if StartSession  ***********
				if(aCommand->getCommandName() == StartSession) {
					world1.setReadiness(ready);
				}

				checker1.checkCommand(aCommand, &world1);	// Test command via the checker
				checker1_code = checker1.getCheckerErr();

				if (checker1_code == CHECKER_OK) {			// ... and the command checks OK
					
					int cmdNm;
					cmdNm = (int)aCommand->getCommandName();


					/************************************************************************/
					/*                                                                      */
					/*   We have a valid (parsed & checked) command.                        */
					/*                                                                      */
					/*   1. If the command is AbortE then clear fast & slow queues          */
					/*   2. Pass the valid command to the executor                          */
					/*                                                                      */
					/************************************************************************/
						
					if(aCommand->getCommandName() == AbortE)	// If Abort clear fast and slow queue
					{ 
						serverQues->clearAllQueues();
					}
				
					executor1->executeCommand(aCommand);	// Pass command to executor

				} // end both parser & checker OK
				


				/************************************************************************/
				/*                                                                      */
				/*   The command parsed OK but failed the checker:                      */
				/*                                                                      */
				/*   1. Get the error code from the checker                             */
				/*   2. Generate an error response                                      */
				/*   3. Check error severity and update world state if required         */
				/*   4. Put error response on the response queue                        */
				/*                                                                      */
				/************************************************************************/

				else 
				{

					// Generate error response
					char errorCauseMethod[IPPSIZE/2]="";
					serverLog.replaceQuotes(cmd);
					if(serverLog.getLogDetails()==true){
						memset(errorMessage, 0, sizeof(errorMessage));
						checker1.getErrorMessageString(errorMessage);
						sprintf(errorCauseMethod,"%s : %s",cmd, errorMessage);
					}
					else
						sprintf(errorCauseMethod,"%s",cmd);
					

					ErrorResponse *_ErrorResponse;
					_ErrorResponse = new ErrorResponse(parser2.getTag(), tt, 
									   (errorNameType)checker1.getIppErr(checker1_code),errorCauseMethod);
					_ErrorResponse->getResponseString(executeBuffer,IPPSIZE-1);
					

					// Check severity
					if (_ErrorResponse->getTheError()->getSeverity() > '1'){
						world1.setReadiness(erred);
						serverQues->clearAllQueues();
					}
					delete _ErrorResponse;				
					
					//Log Ipp error response from checker
					memset(logBuffer, 0, sizeof(logBuffer));
					sprintf(logBuffer, "Sent: %s", executeBuffer);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

					// If set by user, write detailed checker errors to the logfile.
					if(serverLog.getLogDetails() == true){
						memset(logBuffer, 0, sizeof(logBuffer));
						memset(errorMessage, 0, sizeof(errorMessage));
						checker1.getErrorMessageString(errorMessage);
						sprintf(logBuffer, "Checker: %s\n", errorMessage);
						serverLog.writeFile(logBuffer);

						// Write to Socket Message ListBox
						m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());
					}

					// Put on response queue
					while(!serverQues->enqResp(serverQues->getRespKey(), 
						strdup(executeBuffer))){} // Send to response que

					Response * _CompletedResponse = new Response(parser2.getTag(), tt, Complete);
					_CompletedResponse->getResponseString(executeBuffer,IPPSIZE-1);

					delete _CompletedResponse;

					//Log Ipp complete response
					memset(logBuffer, 0, sizeof(logBuffer));
					sprintf(logBuffer, "Sent: %s", executeBuffer);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

					// Put completed response on response queue
					while(!serverQues->enqResp(serverQues->getRespKey(), 
						strdup(executeBuffer))){} // Send to response que

					// Remove current tag from list
					if(parser2.getIsEvent()==1)
						serverQues->delTag(parser2.getTag()*(-1));
					else
						serverQues->delTag(parser2.getTag());
			
				}	
			} 


			/************************************************************************/
			/*                                                                      */
			/*   The command  failed the parser              :                      */
			/*                                                                      */
			/*   1. Get the error code from the parser                              */
			/*   2. Generate an error response                                      */
			/*   3. Check error severity and update world if required               */
			/*   4. Put error response on the response queue                        */
			/*                                                                      */
			/************************************************************************/

			else {  
				
				// Generate error response
				
				char errorCauseMethod[IPPSIZE/2]="";
				serverLog.replaceQuotes(cmd);
				if(serverLog.getLogDetails()==true){
						memset(errorMessage, 0, sizeof(errorMessage));
						parser2.getErrorMessageString(errorMessage);
						sprintf(errorCauseMethod,"%s : %s",cmd, errorMessage);
				}
					else
						sprintf(errorCauseMethod,"%s",cmd);

				ErrorResponse *_ErrorResponse;
				_ErrorResponse = new ErrorResponse(parser2.getTag(), tt, 
					                 (errorNameType)parser2.getIppErr(parser2_code),errorCauseMethod);
				_ErrorResponse->getResponseString(executeBuffer,IPPSIZE-1);

				// Check severity
				if (_ErrorResponse->getTheError()->getSeverity() > '1'){
					world1.setReadiness(erred);
					serverQues->clearAllQueues();
				}
				
				delete _ErrorResponse;
				
				//Log Ipp error response from parser
				memset(logBuffer, 0, sizeof(logBuffer));
				sprintf(logBuffer, "Sent: %s", executeBuffer);
				serverLog.writeFile(logBuffer);

				// Write to Socket Message ListBox
				m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

				// If set by user, write detailed parser errors to the logfile.
				if(serverLog.getLogDetails() == true){
					memset(logBuffer, 0, sizeof(logBuffer));
					memset(errorMessage, 0, sizeof(errorMessage));
					parser2.getErrorMessageString(errorMessage);
					sprintf(logBuffer, "Parser: %s\n", errorMessage);
					serverLog.writeFile(logBuffer);

					// Write to Socket Message ListBox
					m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());
				}

				// Put error response on response queue
				while(!serverQues->enqResp(serverQues->getRespKey(), 
					strdup(executeBuffer))){} // Send to response que


				Response * _CompletedResponse = new Response(parser2.getTag(), tt, Complete);
				_CompletedResponse->getResponseString(executeBuffer,IPPSIZE-1);

				delete _CompletedResponse;

				//Log Ipp completed response 
				memset(logBuffer, 0, sizeof(logBuffer));
				sprintf(logBuffer, "Sent: %s", executeBuffer);
				serverLog.writeFile(logBuffer);

				// Write to Socket Message ListBox
				m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

				// Put completed response on response queue
				while(!serverQues->enqResp(serverQues->getRespKey(), 
					strdup(executeBuffer))){} // Send to response que

				//Remove current tag from list
				if(parser2.getIsEvent()==1)
						serverQues->delTag(parser2.getTag()*(-1));
					else
						serverQues->delTag(parser2.getTag());		
			
			}

		} // End command processing

		
		/********************************************************/
		/*                                                      */
		/*    Process executor responses:                       */
		/*                                                      */
		/*    1. Check the executor for Responses               */
		/*    2. Cast Responses into respective response type   */
		/*    3. Write Responses to Response Queue              */
		/*                                                      */
		/********************************************************/
		
		responseList = executor1->checkStatus();
		responseNameType rnt;

		for (int i=0; i<responseList.size(); i++) {
			
			// Get the response name
			rnt = responseList[i]->getResponseName();

			// Cast response to specific response
			switch (rnt) {

			  case ErrorError				: ErrorResponse *er;
											  er = (ErrorResponse*)responseList[i];
											  er->getResponseString(responseBuffer2,IPPSIZE-1);
											  if (er->getTheError()->getSeverity() > '1'){
												world1.setReadiness(erred);
												serverQues->clearAllQueues();
											  }
											  if(responseList[i]->getTag()->getTagType()==EventTag)
												serverQues->delTag(responseList[i]->getTag()->getTagNumber()*(-1));
											  else
												serverQues->delTag(responseList[i]->getTag()->getTagNumber());
											  break;
			  case Complete                 : responseList[i]->getResponseString(responseBuffer2,IPPSIZE-1);
				                              if(responseList[i]->getTag()->getTagType()==EventTag)
												serverQues->delTag(responseList[i]->getTag()->getTagNumber()*(-1));
											  else
												serverQues->delTag(responseList[i]->getTag()->getTagNumber());
											  //memset(logBuffer, 0, sizeof(logBuffer));
											  //sprintf(logBuffer, "DeleteTag: %d", responseList[i]->getTag()->getTagNumber());
											  //serverLog.writeFile(logBuffer);
				                              break;                      
			  default						: responseList[i]->getResponseString(responseBuffer2,IPPSIZE-1);
											  
			}
			
			//Log executor responses
			memset(logBuffer, 0, sizeof(logBuffer));
			sprintf(logBuffer, "Sent: %s", responseBuffer2);
			serverLog.writeFile(logBuffer);

			// Write to Socket Message ListBox
			m_MessageListBox.InsertString(0, serverLog.getLoggedMessage());

			// Put response on response queue
			while(!serverQues->enqResp(serverQues->getRespKey(),strdup(responseBuffer2))){}

			//delete responseList[i]; ************** add this and test to clean up unused responses

		}

	}


	//
	// Waiting for connection Event called
	//
	else if (nIDEvent == ConnectTimerID)
	{
		

		remoteSocket = accept(listenSocket,			// Listening socket
						  NULL,						// Optional client address
						  NULL);	
		
	
		if (remoteSocket == INVALID_SOCKET)
		{		
			if (WSAEWOULDBLOCK == WSAGetLastError())
			{
				PRINTSTATUS("Waiting for connection ...");
			
			}	
				
			else
			{
				sprintf(szBuf, "Socket error %d", WSAGetLastError());
				PRINTSTATUS(szBuf);
				closesocket(listenSocket);
				closesocket(remoteSocket);
				return;
			}
		}
		else
		{
			// Clear out Socket Message ListBox
			m_MessageListBox.ResetContent();

			SetTimer(ReadTimerID, ReadInterval, 0);
			SetTimer(ExecuteTimerID, ExecuteInterval, 0);
			disconnect = false;
			PRINTSTATUS("Client Connected");
			KillTimer(ConnectTimerID);
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}


void CServerDlg::OnKillfocusErrorCode() 
{
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::OnNoLogParser() 
{
	serverLog.setLogDetails(false);
	PRINTSTATUS("Stop Logging Parser");
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::OnYesLogParser() 
{
	serverLog.setLogDetails(true);
	PRINTSTATUS("Start Logging Parser");
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::OnSelchangeMessageListBox() 
{
	// TODO: Add your control notification handler code here
	
}

void CServerDlg::On_Click_List(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}



void CServerDlg::OnChangeSetProbeRadius() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	if(world1.getInSession()==false){
		CString inText;
		GetDlgItemText(IDC_PROBE_RADIUS, inText);
		sscanf(inText, "%lf", &probe_radius);
		//world1.setProbeRadius(probe_radius);
	}
}

