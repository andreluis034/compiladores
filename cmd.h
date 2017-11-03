#pragma once
#include "list.h"
#include "variable.h"
typedef List CmdList;
struct _Cmd {
	enum {
		C_DECLARATION,
		C_INCREMENT,
		C_IF_ELSE,
		C_FOR,
		C_FUNC_CALL
	} kind;
	union {
		Variable* declaration;
		struct {
			Variable* variable;
			char* operator;
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
			List* variables;
			CmdList* body;
		} funcCall;
	} attr;
};
typedef struct _Cmd Cmd;

CmdList* makeCmdList(Cmd* firstCmd);
CmdList* appendCmd(CmdList* list, Cmd* value);
CmdList* prependCmd(CmdList* list, Cmd* value);
Cmd* getCmd(CmdList* list);
void printCmd(Cmd* cmd, int level, int lastChild);
void printCmdList(CmdList* cmdlist, int level, int lastChild);
void printCmdTree(Cmd* cmd, int level, int lastChild);
Cmd* makeDeclarationCmd(char* varName, Expr* expr);
Cmd* makeIncrementCmd(char* varName, Expr* expr, char* operator);
Cmd* makeIfElseCmd(Expr* expr, CmdList* iftrue, CmdList* iffalse );
Cmd* makeFor(Cmd* initial, Expr* condition, Cmd* afterIteration, CmdList* body );
Cmd* makeFuncCall(char* funcName, List* variables, CmdList* body) ;
