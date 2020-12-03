cmake_minimum_required(VERSION 3.14)
project(riscv_sim)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_FILES main.cpp)
add_executable(riscv_sim ${SOURCE_FILES})

file(GLOB SRC
        "src/*.h"
        "src/*.cpp"
        )
include_directories(src)
add_subdirectory(src)

target_link_libraries(riscv_sim riscv_lib)

add_subdirectory(test)