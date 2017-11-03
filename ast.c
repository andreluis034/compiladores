// AST constructor functions

#include <stdlib.h> // for malloc
#include <stdio.h>
#include "ast.h" // AST header
#include "utility.h"

Expr* ast_variable(char* varName)
{
 	Expr* node = (Expr*) malloc(sizeof(Expr));
	node->kind = E_VARIABLE;
	node->attr.variable = makeVariable(varName, NULL);
	return node;
	return NULL;
}

Expr* ast_integer(int v) {
	Expr* node = (Expr*) malloc(sizeof(Expr));
	node->kind = E_INTEGER;
	node->attr.value = v;
	return node;
}

Expr* ast_bool(int v) {
	Expr* node = (Expr*) malloc(sizeof(Expr));
	node->kind = E_BOOL;
	node->attr.value = v;
	return node;
}

Expr* ast_operation(char* operator, Expr* left, Expr* right) {
	Expr* node = (Expr*) malloc(sizeof(Expr));
	node->kind = E_OPERATION;
	node->attr.op.operator = operator;
	node->attr.op.left = left;
	node->attr.op.right = right;
	return node;
}

void print_variable(Expr* expr, int level, int lastChild)
{
	printVariable(expr->attr.variable, level, lastChild);
}
void print_operation(Expr* expr, int level, int lastChild)
{
	printPadding(level, lastChild);
	printf("E_OPERATION\n");
	printPadding(level + 1, 0);
	printExpr(expr->attr.op.left, level + 1, 0);
	printKeyValue("OPERATOR: ", expr->attr.op.operator, level + 1, 0);
	printExpr(expr->attr.op.right, level + 1, 1);
}

void print_integer(Expr* expr, int level, int lastChild)
{
	printPadding(level, lastChild);
	printf("E_INTEGER\n");
	printPadding(level + 1, 1);
	printf("VALUE: %d\n", expr->attr.value) ;
	
}

void printExpr(Expr* expr, int level, int lastChild)
{
	switch(expr->kind)
	{
		case E_VARIABLE:
			print_variable(expr, level, lastChild);
			break;
		case E_OPERATION:
			break;
	}
}
