cmake_minimum_required(VERSION 3.15)
project(timing)

set(CMAKE_CXX_STANDARD 17)

find_package(benchmark REQUIRED)

add_executable(timing main.cpp)
target_link_libraries(timing benchmark::benchmark)