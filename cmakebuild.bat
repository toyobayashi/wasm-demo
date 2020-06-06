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

set cmakeoutdir=cmake_build

if not exist %cmakeoutdir% mkdir %cmakeoutdir%
cd %cmakeoutdir%
cmake -DCMAKE_TOOLCHAIN_FILE=%EMSCRIPTEN_HOME%\cmake\Modules\Platform\Emscripten.cmake -DCMAKE_BUILD_TYPE=%mode% -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=make ..
cmake --build .
cd ..

copy /Y %cmakeoutdir%\crypto.js .\public\crypto.js
copy /Y %cmakeoutdir%\crypto.wasm .\public\crypto.wasm
copy /Y %cmakeoutdir%\crypto.wasm.map .\crypto.wasm.map
