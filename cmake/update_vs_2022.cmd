@echo off

SET SCRIPT_DIR=%~dp0
cd %SCRIPT_DIR%
cd ..

if exist build\ (rmdir /s/q build)

mkdir build 

echo "Generating project for Visual Studio 2022:"
cmake -G "Visual Studio 17 2022" -A x64 -S "." -B "build" 

pause
