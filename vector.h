#pragma once
#include <stdlib.h>

#define VECTOR_OF(TYPE)  \
    struct               \
    {                    \
        TYPE *data;      \
        size_t size;     \
        size_t capacity; \
    }

#define VECTOR_INIT_CAPACITY(VEC, N)                \
    (VEC).data = malloc((N) * sizeof(*(VEC).data)); \
    (VEC).size = 0;                                 \
    (VEC).capacity = (N);

#define VECTOR_INIT(VEC) VECTOR_INIT_CAPACITY(VEC, 1)

#define VECTOR_SIZE(VEC) (VEC).size

#define VECTOR_CAPACITY(VEC) (VEC).capacity

#define VECTOR_EMPTY(VEC) ((VEC).size == 0)

#define VECTOR_PUSH_BACK(VEC, VAL)                                              \
    if ((VEC).size == (VEC).capacity)                                           \
    {                                                                           \
        (VEC).capacity *= 2;                                                    \
        (VEC).data = realloc((VEC).data, (VEC).capacity * sizeof(*(VEC).data)); \
    }                                                                           \
    (VEC).data[VECTOR_SIZE(VEC)] = (VAL);                                       \
    (VEC).size += 1;

#define VECTOR_ERASE(VEC, INDEX)                                            \
    {                                                                       \
        for (size_t __idx = (INDEX) + 1; __idx < VECTOR_SIZE(VEC); __idx++) \
            (VEC).data[__idx - 1] = (VEC).data[__idx];                      \
        (VEC).size--;                                                       \
    }

#define VECTOR_AT(VEC, INDEX) (VEC).data[INDEX]

#define VECTOR_FRONT(VEC) (VEC).data[0]

#define VECTOR_BACK(VEC) (VEC).data[VECTOR_SIZE(VEC) - 1]

#define VECTOR_FREE(VEC) \
    (VEC).size = 0;      \
    (VEC).capacity = 0;  \
    free((VEC).data);
