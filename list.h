#pragma once
#define IS_EMPTY_LIST(a) (a == NULL)
#define EMPTY_LIST NULL
#define makeTypeList(returnType, funcName, argType) returnType funcName(argType arg){\
    return (returnType)makeList((void*)arg);\
}
#define appendType(returnType, funcName, argType) returnType funcName(returnType list, argType arg){\
    return (returnType)appendToList(list, (void*)arg);\
}
#define prependType(returnType, funcName, argType) returnType funcName(returnType list, argType arg){\
    return (returnType)prependToList(list, (void*)arg);\
}
#define getType(returnType, funcName, argType) returnType funcName(argType list){\
    return (returnType)getPointer(list);\
}
#define concatType(returnType, funcName) returnType funcName(returnType l1, returnType l2){\
    return (returnType)concatList((List*)l1, (List*)l2);\
}

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
List* concatList(List* l1, List* l2);
List* makeCopy(List* l1) ;
List* getPointer(List* list);
List* popIntFromList(List* list);
int peekIntFromList(List* list);
int getNumber(List* list);
