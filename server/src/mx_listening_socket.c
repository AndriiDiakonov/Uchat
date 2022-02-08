#include "../inc/server.h"

static struct sockaddr_in serv_address_description(int port) {
    struct sockaddr_in serv_addr;
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); 
    serv_addr.sin_port        = htons(port);     
    return serv_addr;
}

int mx_listening_socket(int port) {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    mx_error("Creating socket error", socketfd);
    struct sockaddr_in serv_addr = serv_address_description(port);
    int flag = 1;  
    if (-1 == setsockopt(socketfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag))) {  
        perror("setsockopt fail");
    }
    /*if (-1 == setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag))) {  
        perror("setsockopt fail");
    }*/
    int bind_status = bind(socketfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    mx_error("Bind error", bind_status);
    return socketfd;
}
