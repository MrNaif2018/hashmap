#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include <stdbool.h>
#include <unistd.h>

void task4(HashMap *h)
{
    string key = read_string_safe("Enter string to search for: ");
    string result = hashmap_get(h, key);
    if (result == NULL)
        printf("No element found\n");
    else
        printf("%s\n", result);
    free(key);
}