//
// Created by LucaIlari on 4/4/2024.
//
#include <stdio.h>

#include "speed_test.h"
#include "tcp_functions.h"
#include "socket_t.h"

enum StatusCodes startSpeedTest(socket_t sockfd, const int BUFFER_SIZE){
    char *buff = malloc(BUFFER_SIZE * sizeof(char));
    if(buff == NULL)
        return SPEEDTEST_ERROR;

    printf("Starting speedtest\n");
    memset(buff,'A', BUFFER_SIZE-1);
    buff[BUFFER_SIZE-1] = 'B';

    send(sockfd,buff, BUFFER_SIZE/2, 0);
    printf("Half speed test\n");
    send(sockfd,buff+(BUFFER_SIZE/2), BUFFER_SIZE/2, 0);

    printf("Speed test ended\n");
    closeSocket(sockfd);
    return FINISHED;
}