cmake_minimum_required(VERSION 3.0)

add_executable(strip strip.c)
target_link_libraries(strip m)

add_executable(sliding sliding.c)
target_link_libraries(sliding m)

add_executable(rolling rolling.c)
target_link_libraries(rolling m)

add_executable(predator predator.c)
target_link_libraries(predator m)

add_executable(drawing drawing.c)
target_link_libraries(drawing cairo pthread)
