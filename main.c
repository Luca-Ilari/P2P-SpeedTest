#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
typedef u_int64 socket_t;
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
typedef int socket_t;
#endif


int acceptNewConnection(int sockfd){
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

int main(int argc, char* argv[]) {
    int BUFFER_SIZE = 100000000; //100MB buffer
    socket_t sockfd;
    int portno;
    struct sockaddr_in serv_addr;
    portno = 5555;

    if (argc == 1){//Started as server
        printf("Starting server\n");
        sockfd = setupSocket();
        memset(&serv_addr, 0, sizeof(serv_addr));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if (bind(sockfd, (struct sockaddr*)&serv_addr,sizeof(serv_addr)) < 0){
            perror("\nERROR on binding");
            return 0;
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

            closeSocket(newsockfd);
        }
    }else{ // started as client
        printf("Starting client\n");
        sockfd = setupSocket();

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
        serv_addr.sin_port = htons(portno);

        int x = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if (x < 0){
            printf("Error while connecting to server\n");
            return 0;
        }
        printf("Starting speedtest\n");
        char *buff = malloc(BUFFER_SIZE * sizeof(char));
        memset(buff,'A', BUFFER_SIZE-1);
        buff[BUFFER_SIZE-1] = 'B';
        send(sockfd,buff, BUFFER_SIZE/2, 0);
        printf("Half test\n");
        send(sockfd,buff+(BUFFER_SIZE/2), BUFFER_SIZE/2, 0);
        printf("Speed Test ended\n");
        closeSocket(sockfd);
    }
    return 0;
}
