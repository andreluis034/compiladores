#pragma once
#include "list.h"
#include "ast.h"
#include "cmd.h"
typedef List InstList;

typedef enum 
{
	ADD,
	SUB,
	MUL,
	DIV,
	LESS, //BRANCH
	ATRIB
} InstType;

typedef union 
{
	char* str;
	int number;
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

Pair* makePairExpr(Expr* expr);