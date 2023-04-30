#include "hashmap.h"
#include <stdlib.h>
#include "mylib.h"
#include "task1.c"
#include "task2.c"
#include "task3.c"
#include "task4.c"
#include <string.h>

// size_t hashfunc(void *s)
// {
//     return 42;
// }

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

bool compare_strings(void *a, void *b)
{
    return strcomp(a, b);
}

string TASK_DESCRIPTIONS[] = {
    "Read N next lines from text database",
    "Add a new domain",
    "Delete a string",
    "Get ip address by domain"};

void (*TASKS[])() = {task1, task2, task3, task4};

int NUMTASKS = 4;

int main()
{
    HashMap *h = new_hashmap(hashfunc, compare_strings);
    fp = get_file("top-1m.txt", "r"); // use fp from task1
    while (true)
    {
        for (int i = 1; i <= NUMTASKS; i++)
            printf("%i. %s\n", i, TASK_DESCRIPTIONS[i - 1]);
        int task;
        safe_input("Enter task number or any other number to exit: ", scanf("%10i", &task), 1, 1);
        if (task <= 0 || task > NUMTASKS)
            break;
        printf("%s\n", TASK_DESCRIPTIONS[task - 1]);
        int errcode = setjmp(jump_op);
        if (!errcode)
            (*TASKS[task - 1])(h);
        else
            printf("%s\n", strerror(errcode));
    }
    fclose(fp);
    hashmap_free(h);
}