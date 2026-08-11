cmake_minimum_required(VERSION 3.0...3.31)
project(UMATRIX VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
	main.cpp
)

add_compile_definitions(MAIN_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_options(-O3 -march=native)
add_link_options(-O3)

add_executable(UMATRIX ${SOURCES})
