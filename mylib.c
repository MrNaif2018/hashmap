#include "mylib.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <setjmp.h>
#include <unistd.h>
#include <assert.h>

const int SCALE_FACTOR = 2;
const int NOT_FOUND = -1;

size_t length(string s)
{
    size_t i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}

int strcomp(string a, string b)
{
    int alen = length(a), blen = length(b);
    if (alen != blen)
        return 0;
    for (int i = 0; i < alen; i++)
    {
        if (a[i] != b[i])
            return 0;
    }
    return 1;
}

int tolower(int c)
{
    if (c >= 'A' && c <= 'Z')
        c -= ('A' - 'a');
    return c;
}

int toupper(int c)
{
    if (c >= 'a' && c <= 'z')
        c += ('A' - 'a');
    return c;
}

string tolowers(string s)
{
    for (int i = 0; i < (int)length(s); i++)
        s[i] = tolower(s[i]);
    return s;
}

void flush()
{
    while (getchar() != '\n')
        continue;
}

int random_num(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void append(string *s, char c, int *len, int *capacity)
{
    (*len)++;
    if (*capacity < *len)
    {
        *capacity *= SCALE_FACTOR;
        *s = realloc(*s, sizeof(char) * (*capacity));
    }
    (*s)[(*len) - 1] = c;
}

string read_string_safe_generic(string prompt, int (*read_func)())
{
    printf("%s", prompt);
    char c;
    int len = 0;
    int capacity = 1;
    string s = malloc(sizeof(char) * capacity);
    bool read_first = true;
    while (((c = read_func()) != '\n' && c != EOF) || read_first)
    {
        if (c == EOF)
            break;
        if (c == '\n')
            continue;
        read_first = false;
        append(&s, c, &len, &capacity);
    }
    append(&s, '\0', &len, &capacity);
    return s;
}

string read_string_safe(string prompt)
{
    return read_string_safe_generic(prompt, getchar);
}

void get_array(int *a, int n)
{
    for (int i = 0; i < n; i++)
        a[i] = random_num(-100, 100);
    string wants;
    do
    {
        wants = read_string_safe("Do you want to input an array? (Y/n) ");
    } while (!strcomp(tolowers(wants), "y") && !strcomp(tolowers(wants), "n"));
    if (strcomp(wants, "y"))
    {
        for (int i = 0; i < n; i++)
            safe_input("", scanf("%10i", &a[i]), 1, true);
    }
    else
    {
        printf("Random array: ");
        for (int i = 0; i < n; i++)
            printf("%i ", a[i]);
        printf("\n");
    }
    free(wants);
}

int *new_array(int n)
{
    return malloc(sizeof(int) * n);
}

void freearray(int *a)
{
    free(a);
}

int **new_matrix(int n, int m)
{
    int **a = malloc(sizeof(int *) * n);
    for (int i = 0; i < n; i++)
        a[i] = malloc(sizeof(int) * m);
    return a;
}

void freematrix(int **a, int n)
{
    for (int i = 0; i < n; i++)
        free(a[i]);
    free(a);
}

// TODO: reduce duplication
void get_matrix(int **a, int n, int m)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            a[i][j] = random_num(-100, 100);
    }
    string wants;
    do
    {
        wants = read_string_safe("Do you want to input a matrix? (Y/n) ");
    } while (!strcomp(tolowers(wants), "y") && !strcomp(tolowers(wants), "n"));
    if (strcomp(wants, "y"))
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                safe_input("", scanf("%10i", &a[i][j]), 1, true);
        }
    }
    else
    {
        printf("Random matrix:\n");
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
                printf("%i ", a[i][j]);
            printf("\n");
        }
    }
    free(wants);
}

string to_string(int n)
{
    int needed = snprintf(NULL, 0, "%i", n);
    string s = malloc(needed);
    sprintf(s, "%i", n);
    return s;
}

bool isint(string s)
{
    int dotcount = 0, len = length(s);
    for (int i = 0; i < len; i++)
    {
        if ((s[i] == '+' || s[i] == '-') && i != 0)
            return false;
        else if (s[i] == '.' && (++dotcount >= 2 || i == 0 || i == len - 1))
            return false;
        else if ((s[i] < '0' || s[i] > '9') && s[i] != '.' && s[i] != '+' && s[i] != '-')
            return false;
    }
    return true;
}

int max(int a, int b)
{
    return a >= b ? a : b;
}

int min(int a, int b)
{
    return a <= b ? a : b;
}

int find(string s, int v)
{
    for (int i = 0; i < (int)length(s); i++)
    {
        if (s[i] == v)
            return i;
    }
    return NOT_FOUND;
}

int abs(int v)
{
    return v < 0 ? -v : v;
}

int isspace(int c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v' || c == '\f';
}

int ispunct(int c)
{
    if (c >= 128)
        return 0;
    if ((unsigned)c - '!' < '^' && ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
        return 0;
    return 1;
}

int tasks_select_flow(int argc, string *argv, void (*tasks[])(), string *task_descriptions, int numtasks)
{
    if (argc != 2 && argc != 3)
    {
        printf("Usage: %s task [--help]\n", argv[0]);
        return 1;
    }
    string taskname = argv[1];
    if (length(taskname) != 1)
    {
        printf("Invalid task number\n");
        return 1;
    }
    int task = taskname[0] - '0';
    if (task <= 0 || task > numtasks)
    {
        printf("Invalid task number\n");
        return 1;
    }
    if (argc == 3 && (strcomp(argv[2], "-h") || strcomp(argv[2], "--help")))
    {
        printf("%s\n", task_descriptions[task - 1]);
        return 0;
    }
    (*tasks[task - 1])();
    return 0;
}

void *memcpy(void *dest, const void *src, size_t len)
{
    char *d = dest;
    const char *s = src;
    while (len--)
        *d++ = *s++;
    return dest;
}

FILE *get_file(string filename, string mode)
{
    FILE *fp;
    if ((fp = fopen(filename, mode)) == NULL)
        longjmp(jump_op, errno);
    set_current_filename(filename);
    return fp;
}

int get_file_size(FILE *fp)
{
    safe_fseek(fp, 0, SEEK_END);
    int len = safe_ftell(fp);
    rewind(fp);
    return len;
}

static jmp_buf buf __attribute__((unused));
static string filename;

void set_current_filename(string fname)
{
    filename = fname;
}

size_t safe_fwrite(void *ptr, size_t size, size_t n, FILE *fp)
{
    size_t ret = fwrite(ptr, size, n, fp);
    if (!ret)
        longjmp(jump_op, errno);
    if (access(filename, F_OK) != 0)
        longjmp(jump_op, errno);
    return ret;
}

size_t safe_fread(void *ptr, size_t size, size_t n, FILE *fp)
{
    size_t ret = fread(ptr, size, n, fp);
    if (!ret)
        longjmp(jump_op, errno);
    if (access(filename, F_OK) != 0)
        longjmp(jump_op, errno);
    return ret;
}

void safe_fseek(FILE *fp, long off, long whence)
{
    if (fseek(fp, off, whence) == -1)
        longjmp(jump_op, errno);
    if (access(filename, F_OK) != 0)
        longjmp(jump_op, errno);
}

size_t safe_ftell(FILE *fp)
{
    size_t res = ftell(fp);
    if ((int)res == -1)
        longjmp(jump_op, errno);
    if (access(filename, F_OK) != 0)
        longjmp(jump_op, errno);
    return res;
}

string substring(string s, int l, int r)
{
    int slen = length(s);
    assert(l <= r && l >= 0 && r < slen);
    string result = malloc(sizeof(char) * (r - l + 2));
    int idx = 0;
    for (int i = l; i <= r; i++)
        result[idx++] = s[i];
    result[idx] = '\0';
    return result;
}