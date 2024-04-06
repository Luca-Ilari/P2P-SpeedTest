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
#include <netinet/in.h>

#endif

#include "speed_test.h"
#include "tcp_functions.h"
#include "utils.h"

void *startSpeedTest(void *param){
    struct startSpeedTestParams struct_params = *(struct startSpeedTestParams*)param;
    struct sockaddr_in serv_addr = *struct_params.serv_addr;

    int packet_len = 1024;
    char buff[packet_len];

    socket_t socket = setupSocket();

    int x = connect(socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (x < 0){
        perror("Error while connecting to server");
        return NULL;
    }

    printf("Starting speedtest\n");
    memset(buff, 'A', packet_len - 1);

    while(*struct_params.speedtest_ended == 0){
        size_t error = send(socket, buff, packet_len, 0);
        if (error <= 0){
            printf("ERROR ON SEND\nSpeed test failed");
            closeSocket(socket);
            return NULL;
        }
    }
    memset(buff, 'B', packet_len - 1);
    size_t error = send(socket, buff, packet_len, 0);
    if (error <= 0){
        printf("ERROR ON SEND\nSpeed test failed");
        closeSocket(socket);
        return NULL;
    }
    printf("Speed test ended\n");
    closeSocket(socket);
    return NULL;
}

//TODO: Should start a new thread every new socket
/**This function receive the buffer sent by the client.*/
enum StatusCodes receiveSpeedTest(socket_t serverSocket){
    size_t buffer_size = 1024, n;
    char buffer[buffer_size];
    memset(&buffer,'\0', sizeof(buffer));

    socket_t newsockfd = acceptNewConnection(serverSocket);
    if (newsockfd < 0)
        return SPEEDTEST_ERROR;

    printf("Starting speed test\n");

    while(findChar(buffer, 'B', buffer_size) < 0){
        n = recv(newsockfd, buffer, sizeof(buffer), 0);
        if (n <= 0){
            printf("ERROR while testing\n");
            closeSocket(newsockfd);
            return SPEEDTEST_ERROR;
        }
    }

    printf("Speed test finished\n");
    closeSocket(newsockfd);
    return FINISHED;
}