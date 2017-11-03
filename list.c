#include "list.h"
#include <stdlib.h>
#include <stdio.h>
List* makeList(void* Value) 
{
    List* list = (List*)malloc(sizeof(List));
    list->Value = Value;
    list->Next = EMPTY_LIST;
    return list;
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