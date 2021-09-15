@ECHO OFF

RD /S /Q .\LinuxRelease

cd ..
mkdir Output\LinuxRelease
cd Output\LinuxRelease
wsl cmake ../../Src
wsl cmake --build .

pause