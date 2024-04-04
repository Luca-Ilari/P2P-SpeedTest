//
// Created by LucaIlari on 4/4/2024.
//

#ifndef SPEEDTEST_TCP_FUNCTIONS_H
#define SPEEDTEST_TCP_FUNCTIONS_H

#include "socket_t.h"

int acceptNewConnection(socket_t sockfd);
int setupSocket();
void closeSocket(socket_t socket);

#endif //SPEEDTEST_TCP_FUNCTIONS_H
