//
// Created by LucaIlari on 4/4/2024.
//

#ifndef SPEEDTEST_SOCKET_T_H
#define SPEEDTEST_SOCKET_T_H

#ifdef WIN32
#include <windows.h>
#endif

#ifdef WIN32
typedef u_int64 socket_t;
#else
typedef int socket_t;
#endif

#endif //SPEEDTEST_SOCKET_T_H
