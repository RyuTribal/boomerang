@echo off
set /p vs_version="Please enter your Visual Studio version (e.g., vs2019, vs2022): "
call vendor\premake\premake5.exe %vs_version%
PAUSE