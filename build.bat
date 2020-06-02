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

set def=-DAES256=1
set src=.\src\aes\aes.c .\src\crypto.cpp
set CC=emcc
set CXX=em++

if not exist .\dist mkdir .\dist
if /i "%mode%"=="debug" (
  echo %CXX% %src% -o .\dist\crypto.js --bind -g %def% -std=c++11 
  %CXX% %src% -o .\dist\crypto.js --bind -g %def% -std=c++11
) else (
  echo %CXX% %src% -o .\dist\crypto.js --bind -O3 %def% -std=c++11
  %CXX% %src% -o .\dist\crypto.js --bind -O3 %def% -std=c++11
)
