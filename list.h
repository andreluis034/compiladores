#pragma once
#define IS_EMPTY_LIST(a) (a == NULL)
#define EMPTY_LIST NULL
struct _list {
    union {
        int number;
        void* pointer;
    } Value;
    struct _list* Next;    
};

typedef struct _list List;
List* makeList(void* Value);
List* makeInt(int Value);
List* appendToList(List* list, void* value);
List* appendToList(List* list, void* value);
List* prependToList(List* list, void* value) ;
List* prependToListInt(List* list, int value);
List* getPointer(List* list);
List* popIntFromList(List* list);
int peekIntFromList(List* list);
int getNumber(List* list);
