#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "mylib.h"
#include <unistd.h>

void task3(HashMap *h)
{
    string key = read_string_safe("Enter key to delete: ");
    string result = hashmap_remove(h, key);
    if (result == NULL)
        printf("No element found\n");
    else
        printf("Successfully deleted element with value: %s\n", result);
    free(key);
}