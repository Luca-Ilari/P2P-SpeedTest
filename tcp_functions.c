//
// Created by LucaIlari on 4/4/2024.
//
#include <stdio.h>
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#endif
#include "tcp_functions.h"

int acceptNewConnection(socket_t sockfd){
    struct sockaddr_in cli_addr;
    listen(sockfd, 1);
    socklen_t clientLenght = sizeof(cli_addr);
    socket_t newsocket = accept(sockfd, (struct sockaddr*)&cli_addr, &clientLenght);

    if (newsocket < 0){
        return -1;
    }
    char *sockIp= inet_ntoa(cli_addr.sin_addr);

    printf("+ New socket %d connected from: %s\n", newsocket, sockIp);
    return newsocket;
}

int setupSocket(){
#ifdef WIN32
    WSADATA info;
    if (WSAStartup(MAKEWORD(1, 1), &info) == SOCKET_ERROR) {
        perror("ERROR, can't start socket\n");
    }
#endif
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0){
        perror("ERROR opening socket\n");
    }
    return sockfd;
}

void closeSocket(socket_t socket){
#ifdef WIN32
    closesocket(socket);
#else
    close(socket);
#endif
}
