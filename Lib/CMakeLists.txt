cmake_minimum_required(VERSION 3.15)

set(TARGET RawStorage)

file(GLOB_RECURSE SRC *.cpp)

add_library(${TARGET} STATIC ${SRC})
target_include_directories(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Inc)
target_compile_options(${TARGET} PRIVATE ${GLOB_CFLAGS})
target_compile_definitions(${TARGET} PRIVATE ${GLOB_DEFINES})