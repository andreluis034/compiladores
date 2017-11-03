#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"
#include "utility.h"

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

Cmd* makeDeclarationCmd(char* varName, Expr* expr)
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_DECLARATION;
    node->attr.declaration = makeVariable(varName, expr);
    return node;
}

Cmd* makeIncrementCmd(char* varName, Expr* expr, char* operator)
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_INCREMENT;
    node->attr.increment.variable = makeVariable(varName, expr);
    node->attr.increment.operator = operator;
    return node;
}

Cmd* makeIfElseCmd(Expr* expr, CmdList* iftrue, CmdList* iffalse )
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_IF_ELSE;
    node->attr.ifelse.condition = expr;    
    node->attr.ifelse.iftrue = iftrue;    
    node->attr.ifelse.iffalse = iffalse;    
    return node;
}
Cmd* makeFor(Cmd* initial, Expr* condition, Cmd* afterIteration, CmdList* body )
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_FOR;
    node->attr.forCmd.initial = initial;
    node->attr.forCmd.condition = condition;
    node->attr.forCmd.afterIteration = afterIteration;
    return node;
}

Cmd* makeFuncCall(char* funcName, List* variables, CmdList* body) 
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_FUNC_CALL;
    node->attr.funcCall.funcName = funcName;
    node->attr.funcCall.variables = variables;
    node->attr.funcCall.body = body;
    return node;
}

void printDeclaration(Cmd* cmd, int level, int lastChild)  
{
    printVariable(cmd->attr.declaration, level, lastChild);
}

void printIncrement(Cmd* cmd, int level, int lastChild)  
{
    printPadding(level, lastChild);
    printf("C_INCREMENT\n");
    printKeyValue("OPERATOR", cmd->attr.increment.operator, 
    level + 1, 0);
    printVariable(cmd->attr.increment.variable, level +1, 0);
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

void printFor(Cmd* cmd, int level, int lastChild) 
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
void printFuncCall(Cmd* cmd, int level, int lastChild) //TODO
{
    printPadding(level, lastChild);
    printf("C_FUNC_CALL\n");
    printKeyValue("OPERATOR", ":=", level + 1, 0);
    
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
        printCmd(getCmd(cmdlist), level + 1, IS_EMPTY_LIST(cmdlist->Next));
    }
}
void printCmdTree(Cmd* cmd, int level, int lastChild) 
{
    printPadding(level, lastChild);
}
