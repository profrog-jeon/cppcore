@ECHO OFF

SET MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
SET SLNFILE=..\Test\_BuildAll.sln

%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=x64
IF %ERRORLEVEL% NEQ 0 GOTO Error

cd Test
call RunTest.bat
IF %ERRORLEVEL% NEQ 0 GOTO Error

:Success
echo Test succeeded
exit /b 0

:Error
echo Error occurred, code:%ERRORLEVEL%
pause
exit /b -1