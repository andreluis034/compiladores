#pragma once
#include "hashtable.h"

struct _scope
{
    hashmap* hashmap;
    int scope_size;
    int argument_size;
    struct _scope* parent;
};
enum _variableLocation
{
    stack,
    registerr,
};
struct _variable 
{
    char* varName;
    union {
        int registerNumber;
        int stackOffset;
    } position;
    enum _variableLocation location;
};
typedef struct _scope Scope;
typedef enum _variableLocation VariableLocation;
typedef struct _variable Variable;

Scope* createScope();
void addArgument(Scope* s, char* varName, VariableLocation varLocation, int registerNumber);
void addLocalVariable(Scope* s, char* varName);

Variable* getVariableScope(Scope* s, char* varName);