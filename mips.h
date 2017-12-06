#pragma once
#include "threeAddressCode.h"
#include "list.h"

typedef List VariableList;

VariableList* makeVariableList(char* firstVariable);
VariableList* appendVariable(VariableList* list, char* variable);
VariableList* prependVariable(VariableList* list, char* variable);
char* getVariable(VariableList* list);

void compileToMips(InstList* instructionList,CmdList* cmdlist) ;
void initializeRegisters() ;
void checkDeclaration(Cmd* cmd) ;
void checkIfElse(Cmd* cmd) ;
void checkFor(Cmd* cmd) ;
void checkFunc(Cmd* cmd) ;
void checkCmd(Cmd* cmd) ;
void checkCmdList(CmdList* cmdlist);