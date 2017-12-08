#pragma once
#include "list.h"
#include "ast.h"
#include "scope.h"
#define ARG_REGISTER_COUNT 4

typedef List CmdList;
struct _Cmd {
	enum {
		C_DECLARATION,
		C_INCREMENT,
		C_IF_ELSE,
		C_FOR,
		C_FUNC_CALL,
		C_FUNC,
		C_FUNC_RETURN,
		C_RETURN
	} kind;
	union {
		struct {
			Expr* variable;
			char* operator;
			Expr* expr;
		} declaration;
		struct {
			Expr* variable;
			char* operator;
			Expr* expr;
		} increment;
		struct {
			struct _Expr* condition;
			CmdList* iftrue;
			CmdList* iffalse;
        } ifelse;
        struct {
            struct _Cmd* initial;
            struct _Expr* condition;
            struct _Cmd* afterIteration;
            CmdList* body;
        } forCmd;
        struct {
			char* funcName;
			ExprList* variables;
		} funcCall;
		struct {
			Expr* variable;
			char* operator;
			struct _Cmd* funcCall;
		} funcReturn;
		struct {
			char* funcName;
			ExprList* argList;
			CmdList* commandList;
			Scope* scope;
		} func;
		struct {
			Expr* value;
		} _return;
	} attr;
};
typedef struct _Cmd Cmd;

CmdList* makeCmdList(Cmd* firstCmd);
CmdList* appendCmd(CmdList* list, Cmd* value);
CmdList* prependCmd(CmdList* list, Cmd* value);
Cmd* getCmd(CmdList* list);
void printCmd(Cmd* cmd, int level, int lastChild);
void printCmdList(CmdList* cmdlist, int level, int lastChild);
Cmd* makeDeclarationCmd(Expr* variable, char* operator, Expr* expr);
Cmd* makeIncrementCmd(Expr* variable, char* operator, Expr* expr );
Cmd* makeIfElseCmd(Expr* expr, CmdList* iftrue, CmdList* iffalse );
Cmd* makeFor(Cmd* initial, Expr* condition, Cmd* afterIteration, CmdList* body );
Cmd* makeFuncCall(char* funcName, ExprList* variables) ;
Cmd* makeFunc(char* funcName, ExprList* arglist, CmdList* cmdlist) ;
Cmd* makeFunctionReturn(Expr* variable, char* operator, Cmd* funcCall) ;