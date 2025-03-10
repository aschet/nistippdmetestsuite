; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CServerDlg
LastTemplate=CScrollView
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Server.h"

ClassCount=6
Class1=CServerApp
Class2=CServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=viewStatus
Class5=Test
Class6=LogView
Resource3=IDD_SERVER_DIALOG

[CLS:CServerApp]
Type=0
HeaderFile=Server.h
ImplementationFile=Server.cpp
Filter=N

[CLS:CServerDlg]
Type=0
HeaderFile=src\server\serverdlg.h
ImplementationFile=src\server\serverdlg.cpp
BaseClass=CDialog
LastObject=CServerDlg
Filter=D
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=src\server\serverdlg.h
ImplementationFile=src\server\serverdlg.cpp
BaseClass=CDialog
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SERVER_DIALOG]
Type=1
Class=CServerDlg
ControlCount=17
Control1=IDC_SET_LOGFILE,button,1342242816
Control2=IDC_LOGFILE,edit,1350568064
Control3=IDC_STATIC,button,1342178055
Control4=IDCANCEL2,button,1342242816
Control5=IDC_SOCKET_PORT_NUMBER,edit,1350566016
Control6=IDC_STATIC,button,1342178055
Control7=IDC_EXECUTE,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATUS,edit,1350568064
Control10=IDC_STATIC,button,1342178055
Control11=IDC_STATIC,button,1342178055
Control12=IDC_YES_LOG_PARSER,button,1342177289
Control13=IDC_NO_LOG_PARSER,button,1342177289
Control14=IDC_MESSAGE_LIST_BOX,listbox,1353777411
Control15=IDC_STATIC,button,1342178055
Control16=IDC_STATIC,button,1342178055
Control17=IDC_PROBE_RADIUS,edit,1350566016

[CLS:viewStatus]
Type=0
HeaderFile=viewStatus.h
ImplementationFile=viewStatus.cpp
BaseClass=CScrollView
Filter=C

[CLS:Test]
Type=0
HeaderFile=Test.h
ImplementationFile=Test.cpp
BaseClass=CDialog
Filter=D
LastObject=Test

[CLS:LogView]
Type=0
HeaderFile=LogView.h
ImplementationFile=LogView.cpp
BaseClass=CScrollView
Filter=C

