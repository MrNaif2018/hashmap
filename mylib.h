#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "vector.h"
#include <setjmp.h>

typedef char *string;
extern const int NOT_FOUND;
extern const int SCALE_FACTOR;
jmp_buf jump_op;

#define safe_input(prompt, readf, numargs, check)      \
    while (1)                                          \
    {                                                  \
        if (!strcomp(prompt, ""))                      \
            printf("%s", prompt);                      \
        int numread = readf;                           \
        char c = getchar();                            \
        if (numread == numargs && isspace(c) && check) \
            break;                                     \
        if (!isspace(c))                               \
            flush();                                   \
        printf("Bad input\n");                         \
    }

size_t length(string s);

int strcomp(string a, string b);

int tolower(int c);

int toupper(int c);

string tolowers(string s);

void flush();

int random_num(int min, int max);

string read_string_safe_generic(string prompt, int (*read_func)());

string read_string_safe(string prompt);

void get_array(int *a, int n);

int *new_array(int n);

void freearray(int *a);

int **new_matrix(int n, int m);

void freematrix(int **a, int n);

void get_matrix(int **a, int n, int m);

string to_string(int n);

bool isint(string s);

int max(int a, int b);

int min(int a, int b);

int find(string s, int v);

int abs(int v);

int isspace(int c);

int ispunct(int c);

int tasks_select_flow(int argc, string *argv, void (*tasks[])(), string *task_descriptions, int numtasks);

void *memcpy(void *dest, const void *src, size_t len);

FILE *get_file(string filename, string mode);

int get_file_size(FILE *fp);

size_t safe_fread(void *ptr, size_t size, size_t n, FILE *fp);

size_t safe_fwrite(void *ptr, size_t size, size_t n, FILE *fp);

void safe_fseek(FILE *fp, long off, long whence);

size_t safe_ftell(FILE *fp);

void set_current_filename(string fname);

string substring(string s, int l, int r);