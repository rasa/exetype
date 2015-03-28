all:
	VCBuild.exe /nologo exetype.vcproj /rebuild

clean:
	VCBuild.exe /nologo exetype.vcproj /clean
	
upgrade:
	devenv exetype.sln /upgrade

.PHONY:	all clean upgrade

