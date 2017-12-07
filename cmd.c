#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmd.h"
#include "ast.h"
#include "utility.h"

#define sCase(caso, func) case caso: \
func(cmd, level, lastChild);\
break
makeTypeList(CmdList*, makeCmdList, Cmd*)
appendType(CmdList*, appendCmd, Cmd*)
prependType(CmdList*, prependCmd, Cmd*)
getType(Cmd*, getCmd, CmdList*)

/*
Cmd* getCmd(CmdList* list) 
{
    return (Cmd*)getPointer(list);
}
*/
Cmd* makeDeclarationCmd(Expr* variable, char* operator, Expr* expr)
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_DECLARATION;
    node->attr.declaration.variable = variable;// makeVariable(varName, expr);
    node->attr.declaration.operator = operator;
    node->attr.declaration.expr = expr;
    return node;
}

Cmd* makeIncrementCmd(Expr* variable, char* operator, Expr* expr)
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_INCREMENT;
    node->attr.increment.variable = variable;// makeVariable(varName, expr);
    node->attr.increment.operator = operator;
    node->attr.increment.expr = expr;
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

Cmd* makeFuncCall(char* funcName, ExprList* variables) 
{
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_FUNC_CALL;
    node->attr.funcCall.funcName = funcName;
    node->attr.funcCall.variables = variables;
    return node;
}
Cmd* makeFunc(char* funcName, ExprList* arglist, CmdList* cmdlist) 
{
    int regNumber = 0;
    Cmd* node = (Cmd*) malloc(sizeof(Cmd));
    node->kind = C_FUNC;
    node->attr.func.funcName = funcName;
    node->attr.func.argList = arglist;
    node->attr.func.commandList = cmdlist;
    node->attr.func.scope = createScope();
    while(arglist != NULL)
    {
        Expr* expr = getExpr(arglist);
        if (regNumber < ARG_REGISTER_COUNT) 
        {
            addVariable(node->attr.func.scope, expr->attr.variable, registerr, regNumber);
            regNumber++;
        }
        else 
        {
            addVariable(node->attr.func.scope, expr->attr.variable, stack, 0);
        }
        arglist = arglist->Next;
    }
    VariableList* varlist = checkCmdList(cmdlist);
    while(varlist != NULL)
    {
        addVariable(node->attr.func.scope, getVariable(varlist), stack, 0);
        varlist = varlist->Next;
    }
    //printVariableList(cmdlist);
    return node;
}

void printDeclaration(Cmd* cmd, int level, int lastChild)  
{   
    printPadding(level, lastChild);
    printf("C_DECLARATION\n");
    printExpr(cmd->attr.declaration.variable, level+1, 0);
    printKeyValue("OPERATOR", cmd->attr.declaration.operator, level + 1, 0);
    printExpr(cmd->attr.declaration.expr, level+1, 1);
}

void printIncrement(Cmd* cmd, int level, int lastChild)  
{
    printPadding(level, lastChild);
    printf("C_INCREMENT\n");
    printKeyValue("OPERATOR", cmd->attr.increment.operator, 
    level + 1, 0);
    printExpr(cmd->attr.increment.variable, level +1, cmd->attr.increment.expr == NULL);
    if(cmd->attr.increment.expr != NULL)
    {
        printExpr(cmd->attr.increment.expr, level +1, 1);
    }
}
void printIfElse(Cmd* cmd, int level, int lastChild)  
{
    printPadding(level, lastChild);
    printf("C_IF_ELSE\n");
    lastChild = IS_EMPTY_LIST( cmd->attr.ifelse.iffalse );
    printExpr(cmd->attr.ifelse.condition, level + 1, 0);
    printCmdList(cmd->attr.ifelse.iftrue, level + 1, lastChild);
    if(!lastChild)
    {
        printCmdList(cmd->attr.ifelse.iffalse, level + 1, 1);
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
    printExpr(cmd->attr.forCmd.condition, level + 1, 0);
    if(cmd->attr.forCmd.afterIteration != NULL)
    {
        printCmd(cmd->attr.forCmd.afterIteration, level + 1, 0);
    }
    printCmdList(cmd->attr.forCmd.body, level + 1, 1);
}
void printFuncCall(Cmd* cmd, int level, int lastChild) 
{
    printPadding(level, lastChild);
    printf("C_FUNC_CALL\n");
    printKeyValue("NAME", cmd->attr.funcCall.funcName, level + 1, IS_EMPTY_LIST(cmd->attr.funcCall.variables));
    if(!IS_EMPTY_LIST(cmd->attr.funcCall.variables))
    {
        printExprList(cmd->attr.funcCall.variables, level + 1, 1);
    }
}

void printFunc(Cmd* cmd, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("C_FUNC\n");
    printKeyValue("NAME", cmd->attr.func.funcName, level + 1, 0);
    printExprList(cmd->attr.func.argList, level + 1, 0);
    printCmdList(cmd->attr.func.commandList, level + 1, 1);
}

void printCmd(Cmd* cmd, int level, int lastChild)  
{
    switch(cmd->kind)
    {
        sCase(C_DECLARATION, printDeclaration);
        sCase(C_INCREMENT, printIncrement);
        sCase(C_IF_ELSE, printIfElse);
        sCase(C_FOR, printFor);
        sCase(C_FUNC_CALL, printFuncCall);
        sCase(C_FUNC, printFunc);
    }
}

void printCmdList(CmdList* cmdlist, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("CmdList\n");
    while(cmdlist != NULL)
    {
        printCmd(getCmd(cmdlist), level + 1, IS_EMPTY_LIST(cmdlist->Next));
        cmdlist = cmdlist->Next;
    }
}
