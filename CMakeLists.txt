cmake_minimum_required(VERSION 2.8.0)

set(CMAKE_C_FLAGS "-Wall -std=c99 -Wno-unknown-attributes -O2")
include_directories(/usr/local/include)
link_directories(/usr/local/lib)

add_executable(fourcolors main.c)
target_link_libraries(fourcolors gc)
