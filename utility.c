#include <stdio.h>
#include <string.h>
#include "utility.h"

List* skipLevel = EMPTY_LIST;

void pushIgnoredLevel(int level)
{
    skipLevel = prependToListInt(skipLevel, level);
}

int popIgnoredLevel()
{
    int ret = peekIntFromList(skipLevel);
    skipLevel = popIntFromList(skipLevel);
    return ret;
}

int hasInt(List* list, int check)
{
    if(list == NULL)
        return 0;
    if(list->Value.number == check)
    {
      //  printf("has %d\n", check);
        return 1;
    }
    return hasInt(list->Next, check);
}

void popLevels(int level)
{
    while(level <= peekIntFromList(skipLevel)) 
        popIgnoredLevel();
    
}

void printPadding(int level, int lastChild)
{
    if(level == 0)
        return;
    popLevels(level);
    if(lastChild)
        pushIgnoredLevel(level-1);
    char buffer[level*4 + 1] ;
    memset(buffer, 0, sizeof(buffer));
    char buffer1[5] = "|   ";
    char buffer2[5] = "|-- ";
    char buffer3[5] = "|__ ";
    char buffer4[5] = "    ";
    char* finalBuffer = (lastChild == 1 ? buffer3 : buffer2);
    int i;
    for(i = 0; i < (level-1) * 4; i += 4)
    {
        if(hasInt(skipLevel, i/4))
            memcpy(&buffer[i], buffer4, 4);
        else
            memcpy(&buffer[i], buffer1, 4);
    }

    memcpy(&buffer[i], finalBuffer, 4);
    printf("%s", buffer);
}


void printKeyValue(char* prefix, char* suffix, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("%s, %s\n", prefix, suffix);
}

makeTypeList(VariableList*, makeVariableList, char*)
appendType(VariableList*, appendVariable, char*)
prependType(VariableList*, prependVariable, char*)
getType(char*, getVariable, VariableList*)

VariableList* globalVariables;

void checkDeclaration(Cmd* cmd)  
{   
    //printf("%s\n",cmd->attr.declaration.variable->attr.variable);
    if(existsVariable(globalVariables,cmd->attr.declaration.variable->attr.variable)){
    }
    else{
        globalVariables = appendVariable(globalVariables,cmd->attr.declaration.variable->attr.variable);
    }
    
}


void checkIfElse(Cmd* cmd)  
{
    int lastChild = IS_EMPTY_LIST( cmd->attr.ifelse.iffalse );
    checkCmdList(cmd->attr.ifelse.iftrue);
    if(!lastChild)
    {
        checkCmdList(cmd->attr.ifelse.iffalse);
    }
}

void checkFor(Cmd* cmd) 
{

    if(cmd->attr.forCmd.initial != NULL)
    {
        checkCmd(cmd->attr.forCmd.initial);
    }

    if(cmd->attr.forCmd.afterIteration != NULL)
    {
        checkCmd(cmd->attr.forCmd.afterIteration);
    }

    checkCmdList(cmd->attr.forCmd.body);

}

void checkFunc(Cmd* cmd)
{
    //printExprList(cmd->attr.func.argList, level + 1, 0);

    //check args of function

    ExprList* argList = cmd->attr.func.argList;

    while(argList != NULL)
    {
        globalVariables = appendVariable(globalVariables,getExpr(argList)->attr.variable);
        //printf("%s\n",getExpr(argList)->attr.variable);
        argList = argList->Next;
    }

    checkCmdList(cmd->attr.func.commandList);
}

void checkCmd(Cmd* cmd)  
{
    switch(cmd->kind)
    {
        case C_FUNC: 
            checkFunc(cmd);
        break;

        case C_IF_ELSE:
            checkIfElse(cmd);
        break;

        case C_FOR:
            checkFor(cmd);
        break;

        case C_DECLARATION:
            checkDeclaration(cmd);
        break;
    }
}

VariableList* checkCmdList(CmdList* cmdlist)
{
    while(cmdlist != NULL)
    {
        checkCmd(getCmd(cmdlist));
        cmdlist = cmdlist->Next;
    }
    return globalVariables;
}

int existsVariable(VariableList* list, char* toFind)
{
    while(list!=NULL){
        if (strcmp(getVariable(list),toFind) == 0){
            return 1;
        }
        list = list->Next;
    }

    return 0;
}

void printVariableList(VariableList* list)
{
    printf(".data\n");
    while(list!=NULL){
        printf("%s: .word\n",getVariable(list));
        list = list->Next;
    }
}