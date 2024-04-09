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
    socket_t socket;
    int *speedtest_ended;
};

void *uploadToSocket(void *params);
void *downloadFromSocket(void *socketParam);

#endif //SPEEDTEST_SPEED_TEST_H
