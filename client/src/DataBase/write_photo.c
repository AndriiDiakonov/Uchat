#include "../../inc/uchat_client.h"
#include "../../inc/base64.h"

void mx_write_photo_to_bd(char *path, int id){
    if (sockfd == -1){
        mx_connect_to_server(&sockfd);
        //return 1;
    }
    
    char sendBuff[256];
    bzero(sendBuff, 256);
    sprintf(sendBuff, "UpdateAvatar\n%d", id);

    int error = 0;
    socklen_t len = sizeof (error);
    int retval = getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &error, &len);
    if (retval != 0) {
        fprintf(stderr, "error getting socket error code: %s\n", strerror(retval));
        sockfd = -1;
        return;
    }
    if (error != 0) {
        fprintf(stderr, "socket error: %s\n", strerror(error));
        sockfd = -1;
         return;
    }

    if(send(sockfd, sendBuff, 256, 0)  == -1) {
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }

    FILE *fp = fopen(path, "rb");
    int r;

    // Get the length of the file data - 'flen'
    //======================================================
    fseek(fp, 0, SEEK_END);
    if (ferror(fp)) {
        fprintf(stderr, "fseek() failed\n");
        r = fclose(fp);
        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");          
        }    
    }  

    long flen = ftell(fp);
    if (flen == -1) {
        perror("error occurred");
        r = fclose(fp);
        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }   
    }

    fseek(fp, 0, SEEK_SET);
    if (ferror(fp)) {
        fprintf(stderr, "fseek() failed\n");
        r = fclose(fp);
        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }    
    }
    //======================================================

    // Get the data of the file which will be sent to server
    //======================================================
    unsigned char *read_data = malloc((unsigned)flen + 1);
    fread(read_data, flen, 1, fp);
    if (ferror(fp)) {
        fprintf(stderr, "fread() failed\n");
        r = fclose(fp);
        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }    
    }
    //======================================================

    unsigned int out_size = b64e_size(flen) + 1;
    unsigned char *out_b64 = malloc( (sizeof(char) * out_size) );
    mx_memset(out_b64, 0, out_size);
    b64_encode(read_data, flen, out_b64);
    free(read_data);

    int len_encoded = strlen((char *)out_b64);


    if(send(sockfd, &len_encoded, sizeof(int), 0) == -1){
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sockfd = -1;
        return;
    }


    mx_send_all(&sockfd, out_b64, len_encoded);
    free(out_b64); 

    r = fclose(fp);
    if (r == EOF)
        fprintf(stderr, "Cannot close file handler\n");
}
