@echo off
:inicio
grldos.exe
if not exist ejecutar.bat goto fin
call ejecutar.bat
goto inicio
:fin