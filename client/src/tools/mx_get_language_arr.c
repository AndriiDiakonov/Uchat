#include "../../inc/uchat_client.h"

void mx_get_language_arr(void) {
    if (text_for_labels)
        mx_del_strarr(&text_for_labels);
    char *language_from_file = NULL;
    switch (language)
    {
    case 0:
        language_from_file = mx_file_to_str("client/data/eng.txt");
        break;
    case 1:
        language_from_file = mx_file_to_str("client/data/rus.txt");
        break;
    default:
        language_from_file = mx_file_to_str("client/data/ukr.txt");
        break;
    }
    text_for_labels = mx_strsplit(language_from_file, '\n');
    free(language_from_file);
}
