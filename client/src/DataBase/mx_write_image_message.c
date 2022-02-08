#include "../../inc/uchat_client.h"
#include "../../inc/base64.h"

typedef struct s_tmp_data
{
    char *path;
    uintptr_t id;
}              t_tmp_data;

static void *write_image_to_message(void *data);

void mx_write_image_message(char *path, unsigned int id) {
    t_tmp_data *data = (t_tmp_data *)malloc(sizeof(t_tmp_data));
    data->id = (uintptr_t)id;
    data->path = mx_strdup(path);
    pthread_t write_to_message;
    pthread_create(&write_to_message, NULL, write_image_to_message, (void *)data);
}

static void *write_image_to_message(void *data) {
    mx_connect_to_server(&sock_for_send);

    char *path = ((t_tmp_data *)data)->path;
    unsigned int id = (unsigned int)(((t_tmp_data *)data)->id);
    
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
    memset(read_data, 0, flen + 1);
    //fread(read_data, sizeof(unsigned char), flen, fp);
    fread(read_data, flen, 1, fp);
    if (ferror(fp)) {
        fprintf(stderr, "fread() failed\n");
        r = fclose(fp);
        if (r == EOF) {
            fprintf(stderr, "Cannot close file handler\n");
        }
        perror("Error:");
    }
    //======================================================

    unsigned int out_size = b64e_size(flen) + 1;
    unsigned char *out_b64 = malloc( (sizeof(char) * out_size) );
    b64_encode(read_data, flen, out_b64);
    free(read_data);

    int len_encoded = strlen((char *)out_b64);

    char sendBuff[256];
    bzero(sendBuff, 256);
    sprintf(sendBuff, "AddImageMessage\n%d\n%d\n%d\n%u\n%d", t_user.id, (int)curr_destination, 
        (int)id, out_size, len_encoded);

    if (send(sock_for_send, sendBuff, 256, 0) == -1) {
        perror("ERROR writing to socket");
        pthread_t thread_id;
        char *err_msg = "Connection lost\nTry again later";
        pthread_create(&thread_id, NULL, mx_run_error_pop_up, (void *)err_msg); 
        sock_for_send = -1;
        return NULL;
    }
    
    mx_send_all(&sock_for_send, out_b64, len_encoded);

    r = fclose(fp);
    if (r == EOF)
        fprintf(stderr, "Cannot close file handler\n");

    free(((t_tmp_data *)data)->path);
    free((t_tmp_data *)data);

    sleep(3);
    close(sock_for_send);

    return NULL;
}
