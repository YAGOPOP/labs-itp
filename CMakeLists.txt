cmake_minimum_required(VERSION 3.20)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Wshadow
    -Wsign-conversion
    -Wcast-qual
    -Wformat=2
    -Wundef
    -Werror=return-type
    -Wnull-dereference
    -Wdouble-promotion
    -Woverloaded-virtual
    -Wnon-virtual-dtor
)

project(labs-itp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
add_subdirectory(lab4)
add_subdirectory(lab5)
add_subdirectory(lab6)
add_subdirectory(lab7)
add_subdirectory(lab8)
