cmake_minimum_required(VERSION 3.10)
project(lr41 C)

set(CMAKE_C_STANDARD 99)

# Build libraries
add_library(math_lib1 SHARED math_lib1.c)
add_library(math_lib2 SHARED math_lib2.c)

# Build static linking executable
add_executable(main_static main_static.c)
target_link_libraries(main_static math_lib1 m)  # Добавлен флаг "m" для математической библиотеки

# Build dynamic linking executable
add_executable(main_dynamic main_dynamic.c)
target_link_libraries(main_dynamic dl m)
