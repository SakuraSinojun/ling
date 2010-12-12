; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMapEditDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MapEdit.h"
LastPage=0

ClassCount=6
Class1=CMapEditApp
Class2=CMapEditDoc
Class3=CMapEditView
Class4=CMapEditDlg

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDD_DLG_MAPEDIT
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX
Class6=CNewMapDlg
Resource4=IDD_DLG_NEW

[CLS:CMapEditApp]
Type=0
HeaderFile=MapEdit.h
ImplementationFile=MapEdit.cpp
Filter=N

[CLS:CMapEditDoc]
Type=0
HeaderFile=MapEditDoc.h
ImplementationFile=MapEditDoc.cpp
Filter=N

[CLS:CMapEditView]
Type=0
HeaderFile=MapEditView.h
ImplementationFile=MapEditView.cpp
Filter=C


[CLS:CAboutDlg]
Type=0
HeaderFile=MapEdit.cpp
ImplementationFile=MapEdit.cpp
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

[MNU:IDR_MAINFRAME]
Type=1
Class=CMapEditDlg
Command1=ID_LOAD_CONFIG
Command2=ID_LOADMAP
Command3=ID_SAVEMAP
Command4=ID_APP_EXIT
Command5=ID_FILE_NEW
Command6=ID_APP_ABOUT
CommandCount=6

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_DLG_MAPEDIT]
Type=1
Class=CMapEditDlg
ControlCount=3
Control1=IDC_BAR_HORIZON,scrollbar,1342177280
Control2=IDC_BAR_VERTICAL,scrollbar,1342177281
Control3=IDC_COMBO,combobox,1344340227

[CLS:CMapEditDlg]
Type=0
HeaderFile=MapEditDlg.h
ImplementationFile=MapEditDlg.cpp
BaseClass=CDialog
Filter=W
LastObject=ID_SAVEMAP
VirtualFilter=dWC

[DLG:IDD_DLG_NEW]
Type=1
Class=CNewMapDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_WIDTH,edit,1350631552
Control4=IDC_EDIT_HEIGHT,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_CELLSIZE,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_NAME,edit,1350631552
Control10=IDC_STATIC,static,1342308352

[CLS:CNewMapDlg]
Type=0
HeaderFile=NewMapDlg.h
ImplementationFile=NewMapDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_NAME
VirtualFilter=dWC

