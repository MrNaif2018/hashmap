#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mylib.h"
#include <unistd.h>

void task2(HashMap *h)
{
    string key = read_string_safe("Enter string to add: ");
    string value = read_string_safe("Enter value to add: ");
    string result;
    if ((result = hashmap_set(h, key, value)) != NULL)
    {
        free(result);
        free(key);
    }
    printf("Success\n");
}