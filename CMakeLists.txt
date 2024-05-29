cmake_minimum_required (VERSION 3.22)
project(ticket_system)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Ofast")

include_directories(.)
include_directories(src)

add_executable(code src/main.cpp)