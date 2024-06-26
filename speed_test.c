//
// Created by LucaIlari on 4/4/2024.
//
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>

#endif

#include "speed_test.h"
#include "tcp_functions.h"
#include "utils.h"

/**This function send buffer filled with 'A' to socket until the connection drops or param->speedtest_ended is 0*/
void *uploadToSocket(void *param){
    struct startSpeedTestParams struct_params = *(struct startSpeedTestParams*)param;
    socket_t socket = struct_params.socket;

    int packet_len = 1024;
    char buff[packet_len];
    printf("Socket %llu connected\n", socket);
    memset(buff, 'A', packet_len - 1);
    double *bytes_sent = malloc(sizeof(long long int));
    *bytes_sent = 0;
    while(*struct_params.speedtest_ended == 0){
        size_t error = send(socket, buff, packet_len, 0);
        if (error <= 0){
            printf("ERROR ON SEND\nSpeed test failed");
            closeSocket(socket);
            return NULL;
        }
        *bytes_sent += (int)error;
    }
    memset(buff, 'B', packet_len - 1);
    size_t error = send(socket, buff, packet_len, 0);
    if (error <= 0){
        printf("ERROR ON SEND\nSpeed test failed");
        closeSocket(socket);
        return NULL;
    }
    closeSocket(socket);
    return bytes_sent;
}

/**This function receive the buffer sent by the client.*/
void *downloadFromSocket(void *socketParam){
    int socket = (int)socketParam;
    size_t buffer_size = 1024, err;
    char buffer[buffer_size];
    memset(&buffer,'\0', sizeof(buffer));

    printf("Starting speed test\n");

    while(findChar(buffer, 'B', buffer_size) < 0){
        err = recv(socket, buffer, sizeof(buffer), 0);
        if (err <= 0){
            printf("ERROR while testing\n");
            closeSocket(socket);
            return NULL;
        }
    }

    printf("Speed test finished\n");
    closeSocket(socket);
    return NULL;
}