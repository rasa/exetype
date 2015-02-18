# Microsoft Developer Studio Generated NMAKE File, Based on exetype.dsp
!IF "$(CFG)" == ""
CFG=exetype - Win32 Debug
!MESSAGE No configuration specified. Defaulting to exetype - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "exetype - Win32 Release" && "$(CFG)" != "exetype - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "exetype.mak" CFG="exetype - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "exetype - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "exetype - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "exetype - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\exetype.exe"


CLEAN :
	-@erase "$(INTDIR)\exetype.obj"
	-@erase "$(INTDIR)\exetype.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\exetype.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\exetype.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\exetype.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\exetype.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=shared.lib setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\exetype.pdb" /machine:I386 /out:"$(OUTDIR)\exetype.exe" 
LINK32_OBJS= \
	"$(INTDIR)\exetype.obj" \
	"$(INTDIR)\exetype.res"

"$(OUTDIR)\exetype.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "exetype - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\exetype.exe" "$(OUTDIR)\exetype.bsc"


CLEAN :
	-@erase "$(INTDIR)\exetype.obj"
	-@erase "$(INTDIR)\exetype.res"
	-@erase "$(INTDIR)\exetype.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\exetype.bsc"
	-@erase "$(OUTDIR)\exetype.exe"
	-@erase "$(OUTDIR)\exetype.ilk"
	-@erase "$(OUTDIR)\exetype.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\exetype.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\exetype.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\exetype.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\exetype.sbr"

"$(OUTDIR)\exetype.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=sharedd.lib setargv.obj kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\exetype.pdb" /debug /machine:I386 /out:"$(OUTDIR)\exetype.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\exetype.obj" \
	"$(INTDIR)\exetype.res"

"$(OUTDIR)\exetype.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("exetype.dep")
!INCLUDE "exetype.dep"
!ELSE 
!MESSAGE Warning: cannot find "exetype.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "exetype - Win32 Release" || "$(CFG)" == "exetype - Win32 Debug"
SOURCE=.\exetype.cpp

!IF  "$(CFG)" == "exetype - Win32 Release"


"$(INTDIR)\exetype.obj" : $(SOURCE) "$(INTDIR)"


!ELSEIF  "$(CFG)" == "exetype - Win32 Debug"


"$(INTDIR)\exetype.obj"	"$(INTDIR)\exetype.sbr" : $(SOURCE) "$(INTDIR)"


!ENDIF 

SOURCE=.\exetype.rc

"$(INTDIR)\exetype.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

