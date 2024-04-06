#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#endif

#include "tcp_functions.h"
#include "speed_test.h"

void server(int port_number){
    socket_t sockfd;
    //TODO: receive variable len buffer size
    //unsigned int max_speed_test_len = 1000000000; //1000 MB. This is the maximum length of the data sent by the client
    struct sockaddr_in serv_addr;

    printf("Starting server\n");
    sockfd = setupSocket();

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port_number);

    if (bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
        perror("\nERROR on binding");
        return;
    }
    while(1){
        receiveSpeedTest(sockfd);
    }
}

void client(char *ip, unsigned int port_number, unsigned int speed_test_time_second, unsigned int connection_number) {
    struct sockaddr_in serv_addr;

    printf("Starting client\n");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port_number);

    int speed_test_ended = 0;
    pthread_t thread_list[connection_number];
    struct startSpeedTestParams params;
    params.speedtest_ended = &speed_test_ended;
    params.serv_addr = &serv_addr;
    for (int i = 0; i < connection_number; ++i) {
        int err = pthread_create(&thread_list[i], NULL, &startSpeedTest, &params);
        if (err != 0) {
            printf("Error while creating thread: %d", i);
        }
    }
    sleep(speed_test_time_second);
    speed_test_ended = 1;

    for (int i = 0; i < connection_number; ++i){
        pthread_join(thread_list[i],NULL);
    }
}

int main(int argc, char* argv[]) {
    int port_number = 5555;

    if (argc == 1){//Started as server
        server(port_number);
    }else{ // started as client
        client(argv[1], port_number, 5, 4);
    }
    return 0;
}
