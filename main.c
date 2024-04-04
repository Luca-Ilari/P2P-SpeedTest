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
#endif

#include "tcp_functions.h"
#include "speed_test.h"

void server(int port_number){
    socket_t sockfd;
    //TODO: receive variable len buffer size
    int BUFFER_SIZE = 100000000; //100MB buffer
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
        memset(&serv_addr, 0, sizeof(serv_addr));
        socket_t newsockfd = acceptNewConnection(sockfd);

        printf("Starting speed test\n");
        size_t n;
        char *speedTestBuffer = calloc(BUFFER_SIZE, sizeof(char));
        char buffer[1024];
        memset(&buffer,'\0', sizeof(buffer));
        while(speedTestBuffer[strlen(speedTestBuffer)] != 'B'){
            n = recv(newsockfd, buffer, sizeof(buffer), 0);

            if (n <= 0)
                break;
#ifdef WIN32
            strcat_s(speedTestBuffer, n, buffer);
#else
            strlcat(speedTestBuffer,buffer,n);
#endif
        }
        printf("Speed test finished %d\n",n);
        free(speedTestBuffer);
        speedTestBuffer = NULL;
        closeSocket(newsockfd);
    }
}

void client(char *ip, int port_number, int buffer_size_to_send){
    socket_t sockfd;
    struct sockaddr_in serv_addr;

    printf("Starting client\n");
    sockfd = setupSocket();

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port_number);

    int x = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (x < 0){
        printf("Error while connecting to server\n");
        return;
    }
    enum StatusCodes code = startSpeedTest(sockfd, buffer_size_to_send);
}

int main(int argc, char* argv[]) {
    int BUFFER_SIZE = 100000000; //100MB buffer
    int port_number = 5555;

    if (argc == 1){//Started as server
        server(port_number);
    }else{ // started as client
        client(argv[1], port_number, BUFFER_SIZE);
    }
    return 0;
}
