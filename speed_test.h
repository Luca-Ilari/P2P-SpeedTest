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
    struct sockaddr_in *serv_addr;
    int *speedtest_ended;
};

void *startSpeedTest(void *params);
void *receiveSpeedTest(void *socketParam);

#endif //SPEEDTEST_SPEED_TEST_H
