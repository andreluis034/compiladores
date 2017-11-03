#include "list.h"
#include <stdlib.h>
List* makeList(void* Value) 
{
    List* list = (List*)malloc(sizeof(List));
    list->Value = Value;
    list->Next = NULL;
    return list;
}

List* appendToList(List* list, void* value) 
{
    if(IS_EMPTY_LIST(list))
    {
        return makeList(value);
    }
    List* list2;
    for(list2 = list; list2 != NULL; list2 = list2->Next)
    {
        if(list2->Next == NULL)
        {
            list2->Next = makeList(value);
            break;
        }
    }
    return list;
}

List* prependToList(List* list, void* value) 
{
    if(IS_EMPTY_LIST(list))
    {
        return makeList(value);
    }
    List* list2 = makeList(value);
    list2->Next = list;
    return list2;
}