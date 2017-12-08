#include "hashtable.h"
#include <string.h>
#include <stdlib.h>

hashmap* create_hashmap()
{
    int i;
    hashmap* hm = malloc(sizeof(hashmap));
    for(i = 0; i < HASHMAP_SIZE; ++i)
    {
        hm->map[i] = EMPTY_LIST;
    }
    return hm;
}

unsigned long hashString(char* str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    
    return hash % HASHMAP_SIZE;
}

void addKeyValue(hashmap* hm, char* key, void* value) 
{
    int hash = hashString(key);
    hashmap_pair* pair = malloc(sizeof(hashmap_pair));
    pair->key = key;
    pair->value = value;
    hm->map[hash] = prependToList(hm->map[hash], pair);
}

void* getKeyValue(hashmap* hm, char* key) 
{
    int hash = hashString(key);
    List* list = hm->map[hash];
    while(list != NULL)
    {
        hashmap_pair* pair = (hashmap_pair*)list->Value.pointer;
        if(strcmp(pair->key, key) == 0)
            return pair->value;
        list = list->Next;
    }
    return NULL;
}