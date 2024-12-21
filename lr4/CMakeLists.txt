cmake_minimum_required(VERSION 3.10)
project(lr4)

include_directories(${CMAKE_SOURCE_DIR})

add_library(impl1 SHARED impl1.cpp)
add_library(impl2 SHARED impl2.cpp)

add_executable(dlinking d_linking.cpp functions.h)
add_executable(dloading d_loading.cpp functions.h)

target_link_libraries(dlinking impl2)
target_link_libraries(dloading impl1 impl2)