cmake_minimum_required(VERSION 3.15)

set(CMAKE_TOOLCHAIN_FILE toolchain/clang-mingw32.cmake CACHE PATH "" FORCE)

project(DiskAccess VERSION 0.0.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "" FORCE)


set(GLOB_DEFINES
    DEBUG _DEBUG
    # UNICODE _UNICODE
)

set(GLOB_CFLAGS
    # -municode
    -g -O0
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/App)

add_subdirectory(Lib)
add_subdirectory(App)

# https://blog.naver.com/akkdong/150034264033