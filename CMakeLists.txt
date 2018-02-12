cmake_minimum_required(VERSION 3.0)

add_executable(strip strip.c)
target_link_libraries(strip m)

add_executable(sliding sliding.c)
target_link_libraries(sliding m)

add_executable(rolling rolling.c)
target_link_libraries(rolling m)
