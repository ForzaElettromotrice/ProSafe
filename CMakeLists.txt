cmake_minimum_required(VERSION 3.31)
project(ProSafe C)

set(CMAKE_C_STANDARD 17)

add_compile_definitions(Debug)
add_compile_options(-Wall)

add_executable(ProSafe src/main.c)

target_include_directories(ProSafe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

set_target_properties(ProSafe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)