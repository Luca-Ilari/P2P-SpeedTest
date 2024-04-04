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

enum StatusCodes startSpeedTest(socket_t sockfd, const int BUFFER_SIZE);


#endif //SPEEDTEST_SPEED_TEST_H
