@ECHO OFF

SET CUR_DIR=%CD%
SET TEST_DIR=%CUR_DIR%\Test

REM =======================================
REM ===========  WINDOWS BUILD  ===========
REM =======================================
REM SET MSBUILD="C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"
REM SET SLNFILE=..\Test\_BuildAll.sln
REM 
REM %MSBUILD% %SLNFILE% /m /t:Rebuild /p:Configuration=ReleaseMT;Platform=x64
REM IF %ERRORLEVEL% NEQ 0 GOTO Error
REM 
REM cd %TEST_DIR%
REM call RunTest.bat
REM IF %ERRORLEVEL% NEQ 0 GOTO Error

REM =====================================
REM ===========  LINUX BUILD  ===========
REM =====================================
SET LINUX_OUTPUT_DIR=..\Output\LinuxRelease

REM CD %CUR_DIR%
REM RD /S /Q %LINUX_OUTPUT_DIR%

REM mkdir %LINUX_OUTPUT_DIR%
cd %LINUX_OUTPUT_DIR%
wsl cmake ../../Test
wsl cmake --build .
IF %ERRORLEVEL% NEQ 0 GOTO Error

cd %TEST_DIR%
wsl dos2unix ./RunTest.sh
wsl chmod 0755 ./RunTest.sh
wsl ./RunTest.sh

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