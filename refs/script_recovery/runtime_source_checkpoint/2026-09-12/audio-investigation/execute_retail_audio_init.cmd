@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars32.bat" >nul
cl /nologo /EHsc /std:c++17 /MD execute_retail_audio_init.cpp /Fe:execute_retail_audio_init.exe user32.lib /link /BASE:0x400000 /DYNAMICBASE:NO /FIXED
if errorlevel 1 exit /b 1
