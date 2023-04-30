#pragma once
#include <stdlib.h>
#include <stdbool.h>

typedef struct HashMapEntry
{
    void *key;
    void *value;
    struct HashMapEntry *next;
} HashMapEntry;

typedef struct HashMap
{
    HashMapEntry *buckets;
    size_t size;
    size_t capacity;
    size_t max_chain_size;
    size_t (*hashfunc)(void *);
    bool (*compare)(void *, void *);
} HashMap;

HashMap *new_hashmap(size_t (*hashfunc)(void *), bool (*compare)(void *, void *));

void *hashmap_set(HashMap *h, void *key, void *value);

void *hashmap_get(HashMap *h, void *key);

void *hashmap_remove(HashMap *h, void *key);

void hashmap_clear(HashMap *h);

void hashmap_free(HashMap *h);