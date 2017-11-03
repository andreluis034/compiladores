#pragma once
#define IS_EMPTY_LIST(a) (a == NULL)
#define EMPTY_LIST NULL
struct _list {
    void* Value;
    struct _list* Next;    
};

typedef struct _list List;
List* makeList(void* Value);
List* appendToList(List* list, void* value);
List* prependToList(List* list, void* value);