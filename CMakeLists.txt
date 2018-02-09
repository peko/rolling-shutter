cmake_minimum_required(VERSION 3.0)

add_executable(strip strip.c)
target_link_libraries(strip m)
