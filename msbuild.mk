all:
	MSBuild.exe /nologo exetype.sln /p:Configuration=Debug
	MSBuild.exe /nologo exetype.sln /p:Configuration=Release

clean:
	MSBuild.exe /nologo exetype.sln /p:Configuration=Debug   /t:clean
	MSBuild.exe /nologo exetype.sln /p:Configuration=Release /t:clean


upgrade:
	devenv exetype.sln /upgrade
	
.PHONY:	all clean upgrade


