; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSelectSrvDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "netts.h"
LastPage=0

ClassCount=3
Class1=CTWSocket

ResourceCount=6
Resource1=IDD_EDITSERVER
Resource2=IDD_PROXYSET (English (U.S.))
Resource3=IDD_SELECTSERVER
Class2=CSelectSrvDlg
Resource4=IDD_SELECTSERVER (English (U.S.))
Resource5=IDD_PROXYSET
Class3=CEditSrvDlg
Resource6=IDD_EDITSERVER (English (U.S.))

[CLS:CTWSocket]
Type=0
BaseClass=CAsyncSocket
HeaderFile=TWSocket.h
ImplementationFile=TWSocket.cpp
LastObject=CTWSocket
Filter=N
VirtualFilter=q

[DLG:IDD_SELECTSERVER (English (U.S.))]
Type=1
Class=CSelectSrvDlg
ControlCount=15
Control1=IDC_COMBO_SERVER,combobox,1344339971
Control2=IDC_ADD,button,1342275584
Control3=IDC_MODIFY,button,1342275584
Control4=IDC_DELETE,button,1342275584
Control5=IDC_EDIT_USER,edit,1342242944
Control6=IDC_EDIT_PASSWD,edit,1342242976
Control7=IDC_CHECK_SAVEPASSWD,button,1342275587
Control8=IDOK,button,1342275585
Control9=IDC_PROXY_SET,button,1342275584
Control10=IDCANCEL,button,1342275584
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342177287
Control15=IDC_STATIC,button,1342177287

[DLG:IDD_PROXYSET]
Type=1
Class=?
ControlCount=18
Control1=IDC_DIRECT,button,1342210057
Control2=IDC_PRECONFIG,button,1342210057
Control3=IDC_PROXY,button,1342210057
Control4=IDC_PROXY_SOCKS4,button,1342341129
Control5=IDC_PROXY_SOCKS5,button,1342210057
Control6=IDC_PROXY_HTTP,button,1342210057
Control7=IDC_EDIT_PROXYADDRESS,edit,1342242944
Control8=IDC_EDIT_PROXYPORT,edit,1342251136
Control9=IDC_EDIT_PROXYUSER,edit,1342242944
Control10=IDC_EDIT_PROXYPASSWD,edit,1342242976
Control11=IDOK,button,1342275585
Control12=IDCANCEL,button,1342275584
Control13=IDC_STATIC_PROXYPORT,static,1342308352
Control14=IDC_STATIC_PROXYADDRESS,static,1342308352
Control15=65535,static,1342177287
Control16=65535,button,1342177287
Control17=IDC_STATIC_PROXYPASSWD,static,1342308352
Control18=IDC_STATIC_PROXYUSER,static,1342308352

[DLG:IDD_PROXYSET (English (U.S.))]
Type=1
Class=?
ControlCount=18
Control1=IDC_DIRECT,button,1342210057
Control2=IDC_PRECONFIG,button,1342210057
Control3=IDC_PROXY,button,1342210057
Control4=IDC_PROXY_SOCKS4,button,1342341129
Control5=IDC_PROXY_SOCKS5,button,1342210057
Control6=IDC_PROXY_HTTP,button,1342210057
Control7=IDC_EDIT_PROXYADDRESS,edit,1342242944
Control8=IDC_EDIT_PROXYPORT,edit,1342251136
Control9=IDC_EDIT_PROXYUSER,edit,1342242944
Control10=IDC_EDIT_PROXYPASSWD,edit,1342242976
Control11=IDOK,button,1342275585
Control12=IDCANCEL,button,1342275584
Control13=IDC_STATIC_PROXYPORT,static,1342308352
Control14=IDC_STATIC_PROXYADDRESS,static,1342308352
Control15=65535,static,1342177287
Control16=65535,button,1342177287
Control17=IDC_STATIC_PROXYPASSWD,static,1342308352
Control18=IDC_STATIC_PROXYUSER,static,1342308352

[CLS:CSelectSrvDlg]
Type=0
HeaderFile=SelectSrvDlg.h
ImplementationFile=SelectSrvDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CSelectSrvDlg
VirtualFilter=dWC

[DLG:IDD_SELECTSERVER]
Type=1
Class=CSelectSrvDlg
ControlCount=15
Control1=IDC_COMBO_SERVER,combobox,1344339971
Control2=IDC_ADD,button,1342275584
Control3=IDC_MODIFY,button,1342275584
Control4=IDC_DELETE,button,1342275584
Control5=IDC_EDIT_USER,edit,1342242944
Control6=IDC_EDIT_PASSWD,edit,1342242976
Control7=IDC_CHECK_SAVEPASSWD,button,1342275587
Control8=IDOK,button,1342275585
Control9=IDC_PROXY_SET,button,1342275584
Control10=IDCANCEL,button,1342275584
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,static,1342177287
Control15=IDC_STATIC,button,1342177287

[DLG:IDD_EDITSERVER]
Type=1
Class=CEditSrvDlg
ControlCount=15
Control1=IDC_EDIT_NAME,edit,1342242944
Control2=IDC_EDIT_ADDRESS,edit,1342242944
Control3=IDC_EDIT_PORT,edit,1342251136
Control4=IDC_EDIT_USER,edit,1342242944
Control5=IDC_EDIT_PASSWD,edit,1342242976
Control6=IDC_CHECK_SAVEPASSWD,button,1342275587
Control7=IDOK,button,1342275585
Control8=IDCANCEL,button,1342275584
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342177287

[DLG:IDD_EDITSERVER (English (U.S.))]
Type=1
Class=CEditSrvDlg
ControlCount=15
Control1=IDC_EDIT_NAME,edit,1342242944
Control2=IDC_EDIT_ADDRESS,edit,1342242944
Control3=IDC_EDIT_PORT,edit,1342251136
Control4=IDC_EDIT_USER,edit,1342242944
Control5=IDC_EDIT_PASSWD,edit,1342242976
Control6=IDC_CHECK_SAVEPASSWD,button,1342275587
Control7=IDOK,button,1342275585
Control8=IDCANCEL,button,1342275584
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,static,1342177287

[CLS:CEditSrvDlg]
Type=0
HeaderFile=EditSrvDlg.h
ImplementationFile=EditSrvDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CEditSrvDlg
VirtualFilter=dWC

