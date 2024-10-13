cmake_minimum_required(VERSION 3.10)
project(lr2 C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Добавление опций компиляции
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")


add_library(${CMAKE_PROJECT_NAME}_lib src/parallelMergeSort.c)
add_executable(${CMAKE_PROJECT_NAME}_exe main.c)

target_link_libraries(${CMAKE_PROJECT_NAME}_exe PRIVATE ${CMAKE_PROJECT_NAME}_lib)