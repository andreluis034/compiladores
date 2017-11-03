#pragma once
#include "ast.h"
typedef struct _Expr Expr;
typedef struct _variable Variable;
struct _variable 
{
	char* VariableName;
	Expr* value;
};

Variable* makeVariable(char* name, Expr* expr);
void printVariable(Variable* var, int level, int lastChild);