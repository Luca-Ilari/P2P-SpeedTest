//
// Created by LucaIlari on 4/4/2024.
//
#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <string.h>
#include <malloc.h>
#endif

#include "speed_test.h"
#include "tcp_functions.h"

//TODO: should be multi threaded and multi socket?
enum StatusCodes startSpeedTest(socket_t sockfd, int BUFFER_SIZE){
    int packet_size = 1024*100; //Send
    char *buff = malloc(BUFFER_SIZE * sizeof(char));
    if(buff == NULL)
        return SPEEDTEST_ERROR;

    printf("Starting speedtest\n");
    memset(buff,'A', BUFFER_SIZE-1);
    buff[BUFFER_SIZE-1] = 'B';

    for (int  i = 0; i < (BUFFER_SIZE/packet_size); ++i){
        int error = send(sockfd,buff+(i*packet_size), packet_size, 0);
        if (error < 0){
            printf("ERROR ON SEND\nSpeed test failed");
            closeSocket(sockfd);
            return SPEEDTEST_ERROR;
        }
    }
    printf("Speed test ended\n");
    closeSocket(sockfd);
    return FINISHED;
}