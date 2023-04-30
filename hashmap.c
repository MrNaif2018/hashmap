#include <stdlib.h>
#include "mylib.h"
#include "hashmap.h"
#include <stdbool.h>

const long double LOAD_FACTOR = 1; // 0.75;

HashMapEntry *gen_bucket_array(size_t n)
{
    return calloc(n, sizeof(HashMapEntry));
}

void increase_capacity(HashMap *h, size_t factor)
{
    HashMapEntry *oldbuckets = h->buckets;
    size_t oldcapacity = h->capacity;
    h->capacity *= factor;
    h->buckets = gen_bucket_array(h->capacity);
    h->size = 0;
    for (size_t i = 0; i < oldcapacity; i++)
    {
        HashMapEntry *node = &oldbuckets[i];
        if (node->key == NULL) // empty bucket
            continue;
        hashmap_set(h, node->key, node->value);
        node = node->next;
        while (node)
        {
            HashMapEntry *next = node->next;
            hashmap_set(h, node->key, node->value);
            free(node);
            node = next;
        }
    }
    free(oldbuckets);
}

void __check_capacity(HashMap *h)
{
    if (h->size >= LOAD_FACTOR * h->capacity)
        increase_capacity(h, 2);
}

HashMap *new_hashmap(size_t (*hashfunc)(void *), bool (*compare)(void *, void *))
{
    HashMap *map = malloc(sizeof(HashMap));
    map->size = 0;
    map->max_chain_size = 0;
    map->capacity = 1;
    map->buckets = gen_bucket_array(1);
    map->hashfunc = hashfunc;
    map->compare = compare;
    return map;
}

void __node_set(HashMap *h, HashMapEntry *node, void *key, void *value)
{
    if (node->key == NULL)
        h->size++;
    node->key = key;
    node->value = value;
}

void *hashmap_set(HashMap *h, void *key, void *value)
{
    __check_capacity(h);
    size_t hash = h->hashfunc(key), idx = hash % h->capacity;
    HashMapEntry *node = &h->buckets[idx];
    if (node->key == NULL)
    {
        __node_set(h, node, key, value);
        return NULL;
    }
    size_t chain_size = 1;
    do
    {
        if (h->compare(node->key, key))
        {
            void *prev_val = node->value;
            node->value = value;
            return prev_val;
        }
        if (node->next != NULL)
            chain_size++;
    } while (node->next != NULL && (node = node->next));
    h->max_chain_size = max(h->max_chain_size, chain_size);
    node->next = gen_bucket_array(1);
    __node_set(h, node->next, key, value);
    return NULL;
}

void *hashmap_get(HashMap *h, void *key)
{
    size_t hash = h->hashfunc(key), idx = hash % h->capacity;
    HashMapEntry *node = &h->buckets[idx];
    if (node->key == NULL)
        return NULL;
    do
    {
        if (h->compare(node->key, key))
            return node->value;
        node = node->next;
    } while (node != NULL);
    return NULL;
}

void *hashmap_remove(HashMap *h, void *key)
{
    size_t hash = h->hashfunc(key), idx = hash % h->capacity;
    HashMapEntry *node = &h->buckets[idx], *parent = NULL;
    void *result = NULL;
    if (node->key == NULL)
        return result;
    do
    {
        if (!h->compare(node->key, key)) // iterate until we find the key
        {
            parent = node;
            node = node->next;
            continue;
        }
        result = node->value;
        if (parent != NULL)
        {
            parent->next = node->next;
            free(node);
        }
        else
        {
            if (node->next)
            {
                HashMapEntry *tmp = node->next;
                node->key = tmp->key;
                node->value = tmp->value;
                node->next = tmp->next;
                free(tmp);
            }
            else
                node->key = NULL;
        }
        h->size--;
        return result;
    } while (node != NULL);
    return NULL;
}

void __node_empty(HashMap *h, HashMapEntry *node)
{
    if (node)
    {
        __node_empty(h, node->next);
        free(node->key);
        free(node->value);
        free(node);
        h->size--;
    }
}

void hashmap_clear(HashMap *h)
{
    for (size_t i = 0; i < h->capacity; i++)
    {
        HashMapEntry *node = &h->buckets[i];
        if (node->key == NULL)
            continue;
        free(node->key);
        free(node->value);
        node->key = NULL;
        __node_empty(h, node->next);
        node->next = NULL;
        h->size--;
    }
}

void hashmap_free(HashMap *h)
{
    hashmap_clear(h);
    free(h->buckets);
    free(h);
}