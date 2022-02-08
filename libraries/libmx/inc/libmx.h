#ifndef LIBMX_H
#define LIBMX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <wchar.h>
#include <fcntl.h>

typedef struct s_list {
    void *data;
    struct s_list *next;
}              t_list;

void mx_print_unicode(wchar_t c);
void mx_printstr(const char *s);
void mx_print_strarr(char **arr, const char *delim);
double mx_pow(double n, unsigned int pow);
int mx_sqrt(int x);
char *mx_nbr_to_hex(unsigned long nbr);
unsigned long mx_hex_to_nbr(const char *hex);
char *mx_itoa(int number);
int mx_atoi(const char *str);
void mx_foreach(int *arr, int size, void (*f)(int));
int mx_binary_search(char **arr, int size, const char *s, int *count);
int mx_bubble_sort(char **arr, int size);
int mx_quicksort(char **arr, int left, int right);
void mx_swap_char(char *s1, char *s2);
void mx_str_reverse(char *s);
void mx_strdel(char **str);
void mx_del_strarr(char ***arr);
int mx_get_char_index(const char *str, char c);

//functions that server uses
char *mx_strdup(const char *str);
int mx_strlen(const char *s);
void mx_printint(int n);
char *mx_strcpy(char *dst,const char *src);
void mx_check_argv(int argc, char **argv);
void mx_error(char *msg, int status);
void mx_printchar(char c);
char *mx_strnew(const int size);

char *mx_strndup(const char *s1, size_t n);
char *mx_strncpy(char *dst, const char *src, int len);
int mx_strcmp(const char *s1, const char *s2);
char* mx_strcat(char *restricts1, const char *restricts2);
char* mx_strstr(const char *haystack, const char *needle);
int mx_get_substr_index(const char *str, const char *sub);
int mx_count_substr(const char *str, const char *sub);
int mx_count_words(const char *str, char c);
char* mx_strnew(const int size);
char* mx_strtrim(const char *str);
char* mx_del_extra_spaces(const char *str);
char** mx_strsplit(const char *s, char c);
char* mx_strjoin(const char *s1, const char *s2);
char* mx_file_to_str(const char *file);
char* mx_replace_substr(const char *str, const char *sub, const char *replace);
int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd);
void *mx_memset(void *b, int c, size_t len);
void *mx_memcpy(void *restrictdst, const void *restrictsrc, size_t n);
void *mx_memccpy(void *restrictdst, const void *restrictsrc,int c, size_t n);
int mx_memcmp(const void *s1, const void *s2, size_t n);
void* mx_memchr(const void *s, int c, size_t n);
void* mx_memrchr(const void *s, int c, size_t n);
void* mx_memmem(const void*big, size_t big_len, const void *little,size_t little_len);
void* mx_memmove(void *dst, const void *src, size_t len);
void* mx_realloc(void *ptr, size_t size);
t_list *mx_create_node(void *data);
void mx_push_front(t_list **list, void *data);
void mx_push_back(t_list **list, void *data);
void mx_pop_front(t_list **head);
void mx_pop_back(t_list **head);
int mx_list_size(t_list *list);
t_list *mx_sort_list(t_list *lst, bool (*cmp)(void *, void *));

int mx_nbr_length(int num);
int mx_strncmp(const char *s1, const char *s2, int n);
char *mx_str_size_cpy(char *dst, const char *src, int start, int end);
char *mx_str_size_dup_free(char *s1, size_t start, size_t end);
char *mx_str_size_dup(const char *s1, size_t start, size_t end);
bool mx_isspace(char c);
char *mx_strjoin_free(char *s1, const char *s2);
char *mx_strchr(const char *s, int c);
bool mx_isspace(char c);
bool mx_isdigit(int c);
bool mx_isalpha(int c);
bool mx_islower(int c);
bool mx_isupper(int c);

#endif
