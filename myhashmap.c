#include "hashmap.h"
#include <stdlib.h>
#include "mylib.h"
#include <string.h>
#include <sys/time.h>
// #include "murmur3.h"
// #include "city.h"

// size_t hashfunc(void *s)
// {
//     size_t h = 0;
//     for (int i = 0; i < length(s); i++)
//         h += ((string)s)[i];
//     return h;
// }

size_t hashfunc(void *s)
{
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    for (int i = 0; i < (int)length(s); i++)
    {
        hash_value = (hash_value + (((string)s)[i] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return hash_value;
}

// size_t hashfunc(void *s)
// {
//     uint32_t hash[4];
//     MurmurHash3_x64_128(s, length(s), 42, hash);
//     return hash[0];
// }

// size_t hashfunc(void *s)
// {
//     return (size_t)CityHash64((string)s, strlen(s));
// }

bool compare_strings(void *a, void *b)
{
    return strcomp(a, b);
}

FILE *fp;

int read_char()
{
    return fgetc(fp);
}

int main()
{
    HashMap *h = new_hashmap(hashfunc, compare_strings);
    fp = get_file("top-1m.txt", "r");
    string *keys = malloc(sizeof(string)), *values = malloc(sizeof(string));
    int n = 0;
    while (!feof(fp))
    {
        string s = read_string_safe_generic("", read_char);
        int slen = length(s);
        if (slen > 0)
        {
            for (int i = 0; i < slen; i++)
            {
                if (s[i] == ' ')
                {
                    n++;
                    keys = realloc(keys, sizeof(string) * n);
                    values = realloc(values, sizeof(string) * n);
                    string key = substring(s, 0, i - 1), value = substring(s, i + 1, slen - 1);
                    keys[n - 1] = key;
                    values[n - 1] = value;
                    break;
                }
            }
        }
        free(s);
    }
    fclose(fp);
    struct timeval begin, end;
    gettimeofday(&begin, 0);
    for (int i = 0; i < n; i++)
        hashmap_set(h, keys[i], values[i]);
    gettimeofday(&end, 0);
    long seconds = end.tv_sec - begin.tv_sec;
    long microseconds = end.tv_usec - begin.tv_usec;
    double elapsed = seconds + microseconds * 1e-6;
    printf("Time measured: %.3f seconds.\n", elapsed);
    printf("Max chain size: %ld\n", h->max_chain_size);
}