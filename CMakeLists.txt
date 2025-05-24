cmake_minimum_required(VERSION 3.25.5)
project(ProSafe C)

set(CMAKE_C_STANDARD 17)

add_compile_definitions(Debug)
add_compile_options(-Wall)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/BNet)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/CUtils)

add_executable(ProSafe src/main.c)

target_include_directories(ProSafe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ProSafe PRIVATE BNetShared CUtils)
set_target_properties(ProSafe PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
