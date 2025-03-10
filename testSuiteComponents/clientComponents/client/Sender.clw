; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSenderDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Sender.h"

ClassCount=4
Class1=CSenderApp
Class2=CSenderDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SENDER_DIALOG
Class4=coordDlg
Resource4=IDD_COORD_DIALOG

[CLS:CSenderApp]
Type=0
HeaderFile=Sender.h
ImplementationFile=Sender.cpp
Filter=N

[CLS:CSenderDlg]
Type=0
HeaderFile=SenderDlg.h
ImplementationFile=SenderDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CSenderDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=SenderDlg.h
ImplementationFile=SenderDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SENDER_DIALOG]
Type=1
Class=CSenderDlg
ControlCount=25
Control1=IDCANCEL,button,1342242816
Control2=IDC_SET_CMMDRIVER_FILE,button,1342242816
Control3=IDC_CMMDRIVER_FILE,edit,1350568064
Control4=IDC_STATIC,button,1342177287
Control5=IDC_SET_LOGFILE,button,1342242816
Control6=IDC_LOGFILE,edit,1350568064
Control7=IDC_STATIC,button,1342177287
Control8=IDC_HOSTNAME,edit,1350566016
Control9=IDC_STATIC,button,1342177287
Control10=IDC_SOCKET_PORT_NUMBER,edit,1350566016
Control11=IDC_STATIC,button,1342177287
Control12=IDC_EXECUTE,button,1342242816
Control13=IDC_STATIC,button,1342177287
Control14=IDC_STATUS,edit,1350568064
Control15=IDC_STATIC,button,1342177287
Control16=IDC_RADIO1,button,1342308361
Control17=IDC_RADIO2,button,1342177289
Control18=IDC_SINGLE_STEP,button,1342242816
Control19=IDC_SET_MANUAL_COMMAND,button,1342242816
Control20=IDC_MANUAL_COMMAND,edit,1350566016
Control21=IDC_STATIC,button,1342177287
Control22=IDC_CONNECT,button,1342242816
Control23=IDCANCEL2,button,1342242816
Control24=IDC_STATIC,button,1342177287
Control25=IDC_MESSAGE_LIST_BOX,listbox,1353793795

[DLG:IDD_COORD_DIALOG]
Type=1
Class=coordDlg
ControlCount=18
Control1=IDOK,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_X,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_Y,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_Z,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_ROLL,edit,1350631552
Control10=IDC_STATIC,static,1342308352
Control11=IDC_PITCH,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_YAW,edit,1350631552
Control14=IDC_STATIC,button,1342177287
Control15=IDC_RADIO1,button,1342177289
Control16=IDC_TYPE,edit,1082196096
Control17=IDC_RADIO2,button,1342177289
Control18=IDC_RADIO3,button,1342177289

[CLS:coordDlg]
Type=0
HeaderFile=coordDlg.h
ImplementationFile=coordDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_RADIO3
VirtualFilter=dWC

