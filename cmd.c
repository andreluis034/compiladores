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
void printCmd(Cmd* cmd, int level, int lastChild)  
{
    switch(cmd->kind)
    {
        case C_DECLARATION:
            printf("C_DECLARATION, :=\n");
            printPadding(level + 1, 0);
            printf("NAME, %s\n", cmd->attr.declaration.variableName);
            printPadding(level + 1, 1);
            //TODO: print expr
            break;  
        case C_INCREMENT:
            printf("C_INCREMENT\n");
            printPadding(level + 1, 0);
            //PRINT EXPR
            int isNull = cmd->attr.increment.expression == NULL;
            if(isNull)
            {
                printPadding(level + 1, 1);
                //TODO: print expr
            }
            break;
        case C_IF_ELSE:
            printf("C_IF_ELSE, %s\n", cmd->attr.increment.operator);
            int isEmpty = IS_EMPTY_LIST(cmd->attr.ifelse.iffalse) ;
            printPadding(level + 1, isEmpty);
            printCmdList(cmd->attr.ifelse.iftrue, level + 1);
            if(isEmpty)
            {
                printPadding(level + 1, 1);
                printCmdList(cmd->attr.ifelse.iffalse, level + 1);
            
            }
            break;
        case C_FOR:
            break;
        case C_FUNC_CALL:
            break;
        default:
            printf("wtf is going on");
            break;
    }
}

void printCmdList(CmdList* cmdlist, int level)
{
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
