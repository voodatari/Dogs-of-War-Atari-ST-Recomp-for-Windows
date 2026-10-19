@echo off
rem Builds dowport.exe into ..\build (or: build.bat -Out <folder>).  See build.ps1 for the compiler requirements.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0build.ps1" %*
