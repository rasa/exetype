.PHONY:	all clean realclean

all:
	MSBuild.exe exetype.sln /t:Build /p:Configuration=Debug
	MSBuild.exe exetype.sln /t:Build /p:Configuration=Release

clean:
	MSBuild.exe exetype.sln /t:Clean /p:Configuration=Debug
	MSBuild.exe exetype.sln /t:Clean /p:Configuration=Release

realclean: clean
	-cmd /c del /s *.bak *.bsc *.idb *.pdb *.lib *.ncb *.obj *.opt *.pch *.plg *.sbr
