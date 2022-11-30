@ECHO OFF

SET MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
SET SLNFILE=..\Src\_BuildAll.sln

RD /S /Q .\Win32Debug
RD /S /Q .\Win32Release
RD /S /Q .\x64Debug
RD /S /Q .\x64Release

RD /S /Q .\Win32DebugMT
RD /S /Q .\Win32ReleaseMT
RD /S /Q .\x64DebugMT
RD /S /Q .\x64ReleaseMT

%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=Debug;Platform=Win32
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=Release;Platform=Win32
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=Debug;Platform=x64
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=Release;Platform=x64

%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=DebugMT;Platform=Win32
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=Win32
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=DebugMT;Platform=x64
%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=x64

pause