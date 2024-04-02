cmake_minimum_required(VERSION 3.27)
project(SpeedTest C)

set(CMAKE_C_STANDARD 11)

add_executable(SpeedTest main.c)
if (WIN32)
    target_link_libraries(SpeedTest wsock32 ws2_32)
endif ()