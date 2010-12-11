# Microsoft Developer Studio Project File - Name="ling" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ling - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ling.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ling.mak" CFG="ling - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ling - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ling - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ling - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386 /out:"../Release/ling.exe"

!ELSEIF  "$(CFG)" == "ling - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /out:"../Release/ling.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ling - Win32 Release"
# Name "ling - Win32 Debug"
# Begin Group "WIN32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\win32\main.cpp
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\game\Creature.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Creature.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Game.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Game.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Hero.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Hero.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Input.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Input.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Mod.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Mod.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Monster.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Monster.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Npc.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\Npc.h
# End Source File
# Begin Source File

SOURCE=..\src\game\render.cpp
# End Source File
# Begin Source File

SOURCE=..\src\game\render.h
# End Source File
# Begin Source File

SOURCE=..\src\game\Spirit.h
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\src\common\comdef.h
# End Source File
# Begin Source File

SOURCE=..\src\common\common.h
# End Source File
# Begin Source File

SOURCE=..\src\common\datafile.cpp
# End Source File
# Begin Source File

SOURCE=..\src\common\error.cpp
# End Source File
# Begin Source File

SOURCE=..\src\common\preload.cpp
# End Source File
# Begin Source File

SOURCE=..\src\common\typedef.h
# End Source File
# End Group
# End Target
# End Project
