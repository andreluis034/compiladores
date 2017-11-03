#include <stdlib.h>
#include <stdio.h>
#include "variable.h"
#include "utility.h"
Variable* makeVariable(char* name, Expr* expr)
{
	Variable* node = (Variable*) malloc(sizeof(Variable));
	node->VariableName = name;
	node->value = expr;
	return node;
}

void printVariable(Variable* var, int level, int lastChild)
{
	printPadding(level, lastChild);
	printf("VARIABLE\n");
	printKeyValue("NAME", var->VariableName, level + 1, var->value == NULL);
	if(var->value != NULL) {
		printExpr(var->value, level + 1, 1 );
	}
}