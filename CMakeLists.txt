cmake_minimum_required(VERSION 3.6)

project(wasmdemo)

set(EXE_NAME crypto)

set(LIB_SOURCE_FILES
  src/base64.c
  src/crypto.cpp
  src/wasm.cpp
  deps/aes/aes.c
)

add_executable(${EXE_NAME} ${LIB_SOURCE_FILES})

target_link_options(${EXE_NAME} PRIVATE --bind -s ALLOW_MEMORY_GROWTH=1)

set_target_properties(${EXE_NAME} PROPERTIES CXX_STANDARD 11)

# set(CMAKE_EXECUTABLE_SUFFIX ".mjs")

if(${CMAKE_BUILD_TYPE} MATCHES "Debug")
  foreach(var
    CMAKE_C_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_DEBUG
  )
    string(REPLACE "-g" "-g4 --source-map-base http://127.0.0.1:8096/" ${var} "${${var}}")
    message(STATUS "${var}:${${var}}")
  endforeach()
else()
  foreach(var
    CMAKE_C_FLAGS_RELEASE
    CMAKE_C_FLAGS_RELWITHDEBINFO
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_CXX_FLAGS_RELWITHDEBINFO
  )
    string(REPLACE "-O2" "-O3" ${var} "${${var}}")
    message(STATUS "${var}:${${var}}")
  endforeach()
endif()

target_include_directories(${EXE_NAME} PRIVATE "deps/aes")

target_compile_definitions(${EXE_NAME} PRIVATE
  AES256=1
  CBC=1
  CRYPTO_ENC_KEY=\"12345678901234561234567890123456\"
  CRYPTO_DEC_KEY=\"11111222223333344444555556666677\"
)

target_compile_options(${EXE_NAME} PRIVATE -s DISABLE_EXCEPTION_CATCHING=0)
