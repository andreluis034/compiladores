#pragma once
#include "list.h"
#include "cmd.h"
typedef List VariableList;

void printPadding(int level, int lastChild);
void printKeyValue(char* prefix, char* suffix, int level, int lastChild);

VariableList* makeVariableList(char* firstVariable);
VariableList* appendVariable(VariableList* list, char* variable);
VariableList* prependVariable(VariableList* list, char* variable);
char* getVariable(VariableList* list);
void checkDeclaration(Cmd* cmd) ;
void checkIfElse(Cmd* cmd) ;
void checkFor(Cmd* cmd) ;
void checkFunc(Cmd* cmd) ;
void checkCmd(Cmd* cmd) ;
VariableList* checkCmdList(CmdList* cmdlist);
int existsVariable(VariableList* list, char* toFind);
void printVariableList(VariableList* list);