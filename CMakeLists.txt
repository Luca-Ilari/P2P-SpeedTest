cmake_minimum_required(VERSION 3.27)
project(SpeedTest C)

set(CMAKE_C_STANDARD 11)

add_executable(SpeedTest main.c
        tcp_functions.c
        tcp_functions.h
        speed_test.c
        speed_test.h
        socket_t.h
        utils.c
        utils.h)
if (WIN32)
    target_link_libraries(SpeedTest wsock32 ws2_32)
endif ()