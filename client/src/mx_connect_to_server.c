#include "../inc/uchat_client.h"

int mx_connect_to_server(int *sock) {
    int portno;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    
    portno = atoi(argv_ptr[2]);
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    
    if (*sock < 0) {
        perror("ERROR opening socket");
    }
    
    server = gethostbyname(argv_ptr[1]);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        close(*sock);
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(*sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        close(*sock);
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        if (error_revealer == NULL)
            pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        return -1;
    }
    return 0;
}
