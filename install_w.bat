@echo off

mkdir C:\SearchFilesTool
mkdir C:\SearchFilesTool\bin
copy /y /v searchfiles.exe C:\SearchFilesTool\bin\
set PATH=C:\SearchFilesTool\bin;%PATH%