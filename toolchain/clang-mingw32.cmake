SET(tools                       "C:/msys64/mingw32"         CACHE PATH "")

SET(CMAKE_C_COMPILER            "${tools}/bin/clang.exe"    CACHE PATH "")
SET(CMAKE_CXX_COMPILER          "${tools}/bin/clang++.exe"  CACHE PATH "")

SET(CMAKE_AR                    "${tools}/bin/llvm-ar.exe"  CACHE PATH "")
SET(CMAKE_CXX_COMPILER_AR       "${tools}/bin/llvm-ar.exe"  CACHE PATH "")
SET(CMAKE_C_COMPILER_AR         "${tools}/bin/llvm-ar.exe"  CACHE PATH "")
SET(CMAKE_LINKER                "${tools}/bin/llvm-ld.exe"  CACHE PATH "")
