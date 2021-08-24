@ECHO OFF

SET MSBUILD="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\MSBuild\Current\Bin\MSBuild.exe"
SET SLNFILE=..\Src\_BuildAll.sln

RD /S /Q .\Win32DebugMT
RD /S /Q .\Win32ReleaseMT
RD /S /Q .\x64DebugMT
RD /S /Q .\x64ReleaseMT

%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=DebugMT;Platform=Win32
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=Win32
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=DebugMT;Platform=x64
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=x64

pause