#pragma once
#include "list.h"
#define HASHMAP_SIZE 20

typedef struct 
{
    List* map[HASHMAP_SIZE];
} hashmap;

typedef struct 
{
    char* key;
    void* value;
} hashmap_pair;

hashmap* create_hashmap();
void addKeyValue(hashmap* hm, char* key, void* value);
void* getKeyValue(hashmap* hm, char* key);
