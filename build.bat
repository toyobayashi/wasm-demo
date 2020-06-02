@echo off

set mode=
set arch=
set dll=OFF
set staticcrt=false
set test=false

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

set def=-DAES256=1 -DCBC=1
set options=-s DISABLE_EXCEPTION_CATCHING=0 --bind -Wall %def% -std=c++11
set src=.\src\crypto.cpp .\src\wasm.cpp .\src\aes\aes.c
set CC=emcc
set CXX=em++

if not exist .\dist mkdir .\dist
if /i "%mode%"=="debug" (
  echo %CXX% %src% -o .\dist\crypto.js %options% -g
  %CXX% %src% -o .\dist\crypto.js %options% -g
) else (
  echo %CXX% %src% -o .\dist\crypto.js %options% -Os -O3
  %CXX% %src% -o .\dist\crypto.js %options% -Os -O3
)
