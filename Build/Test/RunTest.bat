@ECHO OFF

SET REPORT_DIR=..\Report

RD /S /Q %REPORT_DIR%
MKDIR %REPORT_DIR%
for %%i in (*.exe) do (
	"%%i" --gtest_output=xml:%REPORT_DIR%\%%i.xml
)

wsl chmod 0755 ./RunTest.sh
wsl ./RunTest.sh
pause