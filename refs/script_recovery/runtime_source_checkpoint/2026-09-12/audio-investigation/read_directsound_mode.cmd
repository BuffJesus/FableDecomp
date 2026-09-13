@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" >nul
cl /nologo /EHsc /std:c++17 /MD read_directsound_mode.cpp /Fe:read_directsound_mode.exe ole32.lib dxguid.lib user32.lib
if errorlevel 1 exit /b 1
read_directsound_mode.exe
