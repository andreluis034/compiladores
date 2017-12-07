#include "scope.h"
#include <string.h>
#include <stdlib.h>
Scope* createScope()
{
    Scope* s = malloc(sizeof(Scope));
    //4 bytes for the old stack pointer (0)
    //4 bytes for the return address (-4)
    s->scope_size = 8;
    s->total_size = 8; //includes arguments
    s->hashmap = create_hashmap();
    s->parent = NULL;
    return s;
}

void addVariable(Scope* s, char* varName, VariableLocation varLocation, int registerNumber)
{
    Variable* v = malloc(sizeof(Variable));
    v->varName = varName;
    v->location = varLocation;
    if(v->location == stack) 
    {
        v->position.stackOffset = registerNumber;
        if(registerNumber < 0) {
            //Not the arguments 
            v->position.stackOffset = -s->scope_size;
            s->scope_size += 4;
            
        }
        s->total_size += 4; 
    }
    else
    {
        v->position.registerNumber = registerNumber;
    }
    addKeyValue(s->hashmap, varName, v);
}

Variable* getVariableScope(Scope* s, char* varName)
{
    Variable* v = (Variable*)getKeyValue(s->hashmap, varName);
    if(v == NULL && s->parent != NULL)
        return getVariableScope(s->parent, varName);
    return v;
}