cmake_minimum_required(VERSION 3.16)
project(txbench)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")

add_subdirectory(src)
add_subdirectory(thirdparty)
add_subdirectory(tools)
