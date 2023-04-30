#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"

FILE *fp;

int read_char()
{
    return fgetc(fp);
}

void task1(HashMap *h)
{
    int n;
    safe_input("Enter number of lines to read: ", scanf("%10d", &n), 1, n >= 1);
    while (!feof(fp) && n > 0)
    {
        string s = read_string_safe_generic("", read_char);
        int slen = length(s);
        if (slen > 0)
        {
            for (int i = 0; i < slen; i++)
            {
                if (s[i] == ' ')
                {
                    string key = substring(s, 0, i - 1), value = substring(s, i + 1, slen - 1);
                    hashmap_set(h, key, value);
                    break;
                }
            }
        }
        n--;
        free(s);
    }
}