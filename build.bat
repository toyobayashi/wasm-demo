@echo off

set mode=

:next-arg
if "%1"=="" goto args-done
if /i "%1"=="Debug"         set mode=Debug&goto arg-ok
if /i "%1"=="Release"       set mode=Release&goto arg-ok

:arg-ok
shift
goto next-arg

:args-done
if "%mode%" == "" set mode=Release

echo Mode: %mode%

cd .\deps\aes
make lib CC=emcc LD=emcc AR=emar AES256=1
cd ..\..

cd build
echo make %mode%=1
make %mode%=1
cd ..
