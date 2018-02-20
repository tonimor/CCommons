# Microsoft Developer Studio Project File - Name="CCommons" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 5.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CCommons - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CCommons.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CCommons.mak" CFG="CCommons - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CCommons - Win32 Release" (based on\
 "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MD /W3 /Gm /GX /Zi /Od /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "CCOMMONS_IMPL" /D "_LINK_TO_UTB_IN_EXTDLL" /D "_AFXEXT" /D "_WINDLL" /D "_AFXDLL" /YX"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o NUL /win32
# ADD BASE RSC /l 0x40a /d "NDEBUG"
# ADD RSC /l 0x40a /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 appexcp.lib oodb.lib browdet.lib appbase.lib docs.lib rpexcp.lib secui.lib mfcext.lib oxext.lib taosecur.lib timemod.lib auxext.lib inbrows.lib multdb.lib /nologo /base:"0x10E00000" /subsystem:windows /dll /map /debug /machine:I386 /def:"CCommons.def" /opt:noref /section:.rsrc,rw
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "CCommons - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\CCommons.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgProgressThread.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ISO8859_1Codec.cpp
# End Source File
# Begin Source File

SOURCE=.\ListCtrlWithTooltips.cpp
# End Source File
# Begin Source File

SOURCE=.\qsortint.cpp
# End Source File
# Begin Source File

SOURCE=.\qsortobject.cpp
# End Source File
# Begin Source File

SOURCE=.\qsortstring.cpp
# End Source File
# Begin Source File

SOURCE=.\SplitSelListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\ThreadObject.cpp
# End Source File
# Begin Source File

SOURCE=.\UTF8codec.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\AsyncTask.h
# End Source File
# Begin Source File

SOURCE=.\CCGlobals.h
# End Source File
# Begin Source File

SOURCE=.\CCommons.h
# End Source File
# Begin Source File

SOURCE=.\CCommons_dfdllh1.h
# End Source File
# Begin Source File

SOURCE=.\CCommons_dfdllh2.h
# End Source File
# Begin Source File

SOURCE=.\CCommonsR.h
# End Source File
# Begin Source File

SOURCE=.\DlgProgressThread.h
# End Source File
# Begin Source File

SOURCE=.\GradientProgressCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ISO8859_1Codec.h
# End Source File
# Begin Source File

SOURCE=.\ListCtrlWithTooltips.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\PT.h
# End Source File
# Begin Source File

SOURCE=.\qsortint.h
# End Source File
# Begin Source File

SOURCE=.\qsortobject.h
# End Source File
# Begin Source File

SOURCE=.\qsortstring.h
# End Source File
# Begin Source File

SOURCE=.\SplitSelListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\SQLTask.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stringtokenizer.h
# End Source File
# Begin Source File

SOURCE=.\ThreadObject.h
# End Source File
# Begin Source File

SOURCE=.\UTF8codec.h
# End Source File
# Begin Source File

SOURCE=.\UTF8iterator.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc,bmp,ico,cur"
# Begin Source File

SOURCE=.\res\aero_move.cur
# End Source File
# Begin Source File

SOURCE=.\CCommons.rc
# End Source File
# End Group
# End Target
# End Project
