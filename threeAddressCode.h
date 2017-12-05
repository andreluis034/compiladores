#pragma once
#include "list.h"
#include "ast.h"
#include "cmd.h"
typedef List InstList;

typedef enum 
{
	//Expressions
	ADD,
	SUB,
	MUL,
	DIV,
	EQL,
	OR,	
	AND,//\<|\<\=|\>|\>\=
	LESS,
	LESSEQ,
	MORE,
	MOREEQ,
	//Commands
	MOV,
	LOAD_VARIABLE,
	STORE_VARIABLE,
	LABEL,
	BRANCH_EQ_ZERO,
	BRANCH_NOT_EQ_ZERO,
	GOTO,
	RETURN
} InstType;

typedef struct 
{
	enum {
		S_STR,
		S_INT
	}	type;
	union {
		char* str;
		int number;
	}	symbol;
} InstSymbol;

typedef struct 
{
	InstSymbol* symbol;
	InstList* instructionList;
} Pair;

typedef struct 
{
	InstType type;
	InstSymbol* p1;
	InstSymbol* p2;
	InstSymbol* p3;
} Inst;

typedef struct 
{
	int registerNumber;
	int used;
	char* registerName;
	char* variableRepresented;
} Register;

InstList* compileCommand(Cmd* cmd);
InstList* compileCmdList(CmdList* cmdlist);
Pair* makePairExpr(Expr* expr);