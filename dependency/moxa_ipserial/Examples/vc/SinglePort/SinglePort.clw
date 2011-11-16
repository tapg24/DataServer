; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CInfoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SinglePort.h"

ClassCount=4
Class1=CSinglePortApp
Class2=CSinglePortDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class4=CInfoDlg
Resource3=IDD_SINGLEPORT_DIALOG

[CLS:CSinglePortApp]
Type=0
HeaderFile=SinglePort.h
ImplementationFile=SinglePort.cpp
Filter=N

[CLS:CSinglePortDlg]
Type=0
HeaderFile=SinglePortDlg.h
ImplementationFile=SinglePortDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_DATA_STATUS

[CLS:CAboutDlg]
Type=0
HeaderFile=SinglePortDlg.h
ImplementationFile=SinglePortDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SINGLEPORT_DIALOG]
Type=1
Class=CSinglePortDlg
ControlCount=41
Control1=IDC_SERVER_IP,edit,1350631552
Control2=IDC_PORT,edit,1350631554
Control3=IDC_BAUD_RATE,combobox,1344339971
Control4=IDC_DATA_BITS,combobox,1344339971
Control5=IDC_STOP_BITS,combobox,1344339971
Control6=IDC_PARITY,combobox,1344339971
Control7=IDC_FLOW_CONTROL,combobox,1344339971
Control8=IDC_DTR_ON,button,1342308361
Control9=IDC_DTR_OFF,button,1342177289
Control10=IDC_RTS_ON,button,1342308361
Control11=IDC_RTS_OFF,button,1342177289
Control12=IDC_TRANSCEIVER,edit,1353777156
Control13=IDC_OPEN,button,1342242816
Control14=IDC_CLOSE,button,1342242816
Control15=IDC_RESET_PORT,button,1342242816
Control16=IDC_RESET_SERVER,button,1342242816
Control17=IDC_CHECK_ALIVE,button,1342242816
Control18=IDC_SEND_BREAK,button,1342242816
Control19=IDOK,button,1208025089
Control20=IDCANCEL,button,1342242816
Control21=IDC_STATIC,static,1342308352
Control22=IDC_STATIC,static,1342308352
Control23=IDC_STATIC,static,1342308352
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,button,1342178055
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,button,1342178055
Control32=IDC_STATIC,static,1342308352
Control33=IDC_STATIC,static,1342308352
Control34=IDC_STATIC,static,1342308352
Control35=IDC_STATIC,button,1342178055
Control36=IDC_STATIC,static,1342308352
Control37=IDC_CTS_CTRL,edit,1484783744
Control38=IDC_DSR_CTRL,edit,1484783744
Control39=IDC_DCD_CTRL,edit,1484783744
Control40=IDC_BREAK_CTRL,edit,1484791938
Control41=IDC_DATA_STATUS,button,1476460544

[CLS:CInfoDlg]
Type=0
HeaderFile=InfoDlg.h
ImplementationFile=InfoDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CInfoDlg

