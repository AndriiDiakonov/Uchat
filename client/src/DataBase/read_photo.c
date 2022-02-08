#include "../../inc/uchat_client.h"
#include "../../inc/base64.h"

void mx_read_photo_from_bd(int id) {
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }
    
    FILE *fp = fopen("client/img/tmp_avatar.png", "wb");
    if (fp == NULL)
        fprintf(stderr, "Cannot open image file\n");

    char sendBuff[256];
    sprintf(sendBuff, "GetAvatar\n%d", id);

    if(send(sockfd, sendBuff, 256, 0) == -1) {
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    int len_encoded = 0;


    if(recv(sockfd, &len_encoded, sizeof(int), 0) == 0){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    unsigned char *encoded = malloc( (sizeof(char) * len_encoded) );
    mx_memset(encoded, 0, len_encoded);
    mx_recv_all(&sockfd, &encoded, len_encoded);

    unsigned int flen = b64d_size(len_encoded);
    unsigned char *decoded = malloc( (sizeof(char) * flen) + 1);
    flen = b64_decode(encoded, len_encoded, decoded);        
    free(encoded);

    fwrite(decoded, flen, 1, fp);
    if (ferror(fp))
        fprintf(stderr, "fwrite() failed\n");
    
    int r = fclose(fp);
    if (r == EOF)
        fprintf(stderr, "Cannot close file handler\n");

    free(decoded);
}
