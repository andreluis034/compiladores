#pragma once
#include "list.h"

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
		struct {
			char* variableName;
			struct _Expr* expression;
		} declaration;
		struct {
			char* variableName;
			char* operator;
			struct _Expr* expression;
		} increment;
		struct {
			struct _Expr* condition;
			CmdList* iftrue;
			CmdList* iffalse;
        } ifelse;
        struct {
            struct _Cmd* initial;
            struct _Expr* condition;
            struct _Cmd* afterLoop;
            CmdList* body;
        } forCmd;
        struct {
            char* funcName;
        } funcCall;
	} attr;
};
typedef struct _Cmd Cmd;

CmdList* makeCmdList(Cmd* firstCmd);
CmdList* appendCmd(CmdList* list, Cmd* value);
CmdList* prependCmd(CmdList* list, Cmd* value);
Cmd* getCmd(CmdList* list);