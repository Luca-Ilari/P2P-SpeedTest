//
// Created by LucaIlari on 4/4/2024.
//

#ifndef SPEEDTEST_SPEED_TEST_H
#define SPEEDTEST_SPEED_TEST_H

#include "socket_t.h"

enum StatusCodes{
    FINISHED = -1,
    SPEEDTEST_ERROR = 0
};

struct startSpeedTestParams{
    //char *ip;
    //unsigned int port_number;
    struct sockaddr_in *serv_addr;
    int *speedtest_ended;
};

void *startSpeedTest(void *params);
enum StatusCodes receiveSpeedTest(socket_t serverSocket);

#endif //SPEEDTEST_SPEED_TEST_H
