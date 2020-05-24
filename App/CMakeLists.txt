cmake_minimum_required(VERSION 3.15)

set(TARGET DiskAccess)

set(INC
    ${PROJECT_SOURCE_DIR}/Lib/Inc 
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

set(LIB
    RawStorage
)

file(GLOB_RECURSE SRC *.cpp)

add_executable(${TARGET} ${SRC})

target_include_directories(${TARGET} PRIVATE ${INC})
target_compile_options(${TARGET} PRIVATE ${GLOB_CFLAGS})
target_compile_definitions(${TARGET} PRIVATE ${GLOB_DEFINES})
target_link_libraries(${TARGET} PRIVATE ${LIB})

set_target_properties(${TARGET} PROPERTIES WIN32_EXECUTABLE NO)