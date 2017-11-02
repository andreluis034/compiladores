#include "cmd.h"
#include <stdio.h>
#include <string.h>
CmdList* makeCmdList(Cmd* firstCmd)
{
    return (CmdList*)makeList((void*)firstCmd);
}
CmdList* appendCmd(CmdList* list, Cmd* value) 
{
    return (CmdList*)appendToList(list, (void*)value);
}
CmdList* prependCmd(CmdList* list, Cmd* value) 
{
    return (CmdList*)prependToList(list, (void*)value);    
}

Cmd* getCmd(CmdList* list) 
{
    return (Cmd*)list->Value;
}
void printPadding(int level, int lastChild)
{
    if(level == 0)
        return;
    char buffer[level*4 + 1] ;
    memset(buffer, 0, sizeof(buffer));
    char buffer1[6] = "|   ";
    char buffer2[6] = "|-- ";
    char buffer3[6] = "|__ ";
    char* finalBuffer = (lastChild == 1 ? buffer3 : buffer2);
    int i;
    for(i = 0; i < (level-1) * 4; ++i)
    {
        buffer[i] = buffer1[i%4];
    }

    memcpy(&buffer[i], finalBuffer, 4);
    printf("%s\n", buffer);
}

void printStringName(char* prefix, char* suffix, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("%s, %s\n", prefix, suffix);
}

void printDeclaration(Cmd* cmd, int level, int lastChild)  
{
    printPadding(level, lastChild);
    printf("C_DECLARATION\n");
    printStringName("OPERATOR", ":=", level + 1, 0);
    printStringName("VARNAME", cmd->attr.declaration.variableName, level + 1, 0);
    //printPadding(level + 1, 1);
    //TODO PRINT DECLARATION
}

void printIncrement(Cmd* cmd, int level, int lastChild)  
{
    printPadding(level, lastChild);
    printf("C_INCREMENT\n");
    printStringName("OPERATOR", cmd->attr.increment.operator, 
        level + 1, cmd->attr.increment.expression == NULL);
    if( cmd->attr.increment.expression != NULL)
    {
        //TODO print EXPR
    }
}
void printIfElse(Cmd* cmd, int level, int lastChild)  
{
    printPadding(level, lastChild);
    printf("C_IF_ELSE\n");
    //TODO print EXPR
    lastChild = IS_EMPTY_LIST( cmd->attr.ifelse.iffalse );
    printCmdList(cmd->attr.ifelse.iftrue, level + 1, lastChild);
    if(!lastChild)
    {
        printCmdList(cmd->attr.ifelse.iffalse, level + 1, lastChild);
    }
}

printFor(Cmd* cmd, int level, int lastChild) 
{
    printPadding(level, lastChild);
    printf("C_FOR\n");
    if(cmd->attr.forCmd.initial != NULL)
    {
        printCmd(cmd->attr.forCmd.initial, level + 1, 0);
    }
    //TODO print EXPR
    //printCmd(cmd->attr.forCmd.condition, level + 1, cmd->attr.forCmd.afterIteration != NULL);
    if(cmd->attr.forCmd.afterIteration != NULL)
    {
        printCmd(cmd->attr.forCmd.afterIteration, level + 1, 1);
    }
}
printFuncCall(Cmd* cmd, int level, int lastChild) 
{
    printPadding(level, lastChild);
    printf("C_FUNC_CALL\n");
    printStringName("OPERATOR", ":=", level + 1, 0);
    
}

void printCmd(Cmd* cmd, int level, int lastChild)  
{
    switch(cmd->kind)
    {
        case C_DECLARATION:
            printDeclaration(cmd, level, lastChild);
            break;  
        case C_INCREMENT:
            printIncrement(cmd, level, lastChild);
            break;
        case C_IF_ELSE:
            printIfElse(cmd, level, lastChild);
            break;
        case C_FOR:
            printFor(cmd, level, lastChild);
            break;
        case C_FUNC_CALL:
            break;
        default:
            printf("wtf is going on");
            break;
    }
}

void printCmdList(CmdList* cmdlist, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("CmdList\n");
    while(cmdlist != NULL)
    {
        printCmdTree(getCmd(cmdlist), level + 1, IS_EMPTY_LIST(cmdlist->Next));
    }
}
void printCmdTree(Cmd* cmd, int level, int lastChild) 
{
    printPadding(level, lastChild);
}
