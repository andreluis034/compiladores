#include "list.h"
#include <stdlib.h>
#include <stdio.h>
int getNumber(List* list)
{
    return list->Value.number;
}

List* getPointer(List* list)
{
    return (List*)list->Value.pointer;
}
List* makeList(void* Value) 
{
    List* list = (List*)malloc(sizeof(List));
    list->Value.pointer = Value;
    list->Next = EMPTY_LIST;
    return list;
}
List* makeInt(int Value)
{

    List* list = makeList(NULL);
    list->Value.number = Value;
    return list;
}
List* concatList(List* l1, List* l2) 
{
    List* copy1 = makeCopy(l1);
    List* start1 = copy1;
    while(copy1->Next != NULL)
        copy1 = copy1->Next;
    copy1->Next = makeCopy(l2);
    return start1;
}
List* makeCopy(List* l1) 
{
    List* l2 = EMPTY_LIST;
    while(l1 != NULL) {
        l2 = appendToList(l2, l1->Value.pointer);
        l1 = l1->Next;
    }
    return l2;
}
List* appendToList(List* list, void* value) 
{
    if(IS_EMPTY_LIST(list))
    {
        return makeList(value);
    }
    List* list2;
    for(list2 = list; list2 != EMPTY_LIST; list2 = list2->Next)
    {
        if(list2->Next == EMPTY_LIST)
        {
            list2->Next = makeList(value);
            break;
        }
    }
    return list;
}
List* prependToList(List* list, void* value) 
{
    List* list2 = makeList(value);
    if(IS_EMPTY_LIST(list))
    {
        return list2;
    }
    list2->Next = list;
    return list2;
}
List* prependToListInt(List* list, int value)
{
     list = prependToList(list, NULL);
     list->Value.number = value;
     return list;
}



List* popIntFromList(List* list)
{
    if(list == NULL)
        return NULL;
    List* list2 = list->Next;
    free(list);
    return list2;
}

int peekIntFromList(List* list)
{
    if(list == NULL)
        return -1;
    return getNumber(list);
}

