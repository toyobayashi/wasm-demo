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

set def=-DAES256=1 -DCBC=1
set options=-s DISABLE_EXCEPTION_CATCHING=0 --bind -Wall %def% -std=c++11 -Ideps/aes
set src=.\src\crypto.cpp .\src\wasm.cpp .\deps\aes\aes.a
set CC=emcc
set CXX=em++

if not exist .\dist mkdir .\dist
if /i "%mode%"=="debug" (
  echo %CXX% %src% -o .\dist\crypto.js %options% -g
  %CXX% %src% -o .\dist\crypto.js %options% -g
) else (
  echo %CXX% %src% -o .\dist\crypto.js %options% -O3
  %CXX% %src% -o .\dist\crypto.js %options% -O3
)
