#include "../../inc/uchat_client.h"

void mx_sort_string(char *data) {
    int len = mx_strlen(data);

    for (int i = 0; i < len - 1; i++) {
        for (int j = i + 1; j < len; j++) {
            if (data[i] < data[j]) {
                char temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
}
