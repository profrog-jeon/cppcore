@ECHO OFF

SET CUR_DIR=%CD%
SET TEST_DIR=%CD%\Test

REM =======================================
REM ===========  WINDOWS BUILD  ===========
REM =======================================
SET MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
SET SLNFILE=..\Test\_BuildAll.sln

%MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=x64
IF %ERRORLEVEL% NEQ 0 GOTO Error

cd %TEST_DIR%
call RunTest.bat
IF %ERRORLEVEL% NEQ 0 GOTO Error

REM =====================================
REM ===========  LINUX BUILD  ===========
REM =====================================
SET LINUX_OUTPUT_DIR=..\Output\LinuxRelease

cd %CUR_DIR%
rd /S /Q %LINUX_OUTPUT_DIR%

mkdir %LINUX_OUTPUT_DIR%
cd %LINUX_OUTPUT_DIR%
wsl cmake ../../Test
wsl cmake --build .
IF %ERRORLEVEL% NEQ 0 GOTO Error

cd %TEST_DIR%
wsl dos2unix ./RunTest.sh
wsl chmod 0755 ./RunTest.sh
wsl ./RunTest.sh
IF %ERRORLEVEL% NEQ 0 GOTO Error

REM ==================================
REM ===========  FINALIZE  ===========
REM ==================================
:Success
echo Test succeeded
exit /b 0

:Error
echo Error occurred, code:%ERRORLEVEL%
pause
exit /b -1