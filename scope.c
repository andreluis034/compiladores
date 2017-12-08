#include "scope.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
Scope* createScope()
{
    Scope* s = malloc(sizeof(Scope));
    //4 bytes for the old stack pointer (0)
    //4 bytes for the return address (-4)
    s->scope_size = 8;
    s->argument_size = 0;

    s->hashmap = create_hashmap();
    s->parent = NULL;
    return s;
}

void addArgument(Scope* s, char* varName, VariableLocation varLocation, int registerNumber)
{
    Variable* v = malloc(sizeof(Variable));
    v->varName = varName;
    v->location = varLocation;
    if(v->location == stack) 
    {  
        v->position.stackOffset = s->argument_size;
        s->argument_size += 4; 
    }
    else
    {
        v->position.registerNumber = registerNumber;
    }
    addKeyValue(s->hashmap, varName, v);
}

void addLocalVariable(Scope* s, char* varName)
{
    s->scope_size += 4;
    Variable* v = malloc(sizeof(Variable));
    v->varName = varName;
    v->location = stack;
    v->position.stackOffset = -s->scope_size;

    addKeyValue(s->hashmap, varName, v);
}

Variable* getVariableScope(Scope* s, char* varName)
{
    Variable* v = (Variable*)getKeyValue(s->hashmap, varName);
    if(v == NULL && s->parent != NULL)
        return getVariableScope(s->parent, varName);
    return v;
}