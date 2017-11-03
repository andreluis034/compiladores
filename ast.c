// AST constructor functions

#include <stdlib.h> // for malloc
#include <stdio.h>
#include "ast.h" // AST header
#include "utility.h"

ExprList* makeExprList(Expr* firstExpr)
{
    return (ExprList*)makeList((void*)firstExpr);
}
ExprList* appendExpr(ExprList* list, Expr* value) 
{
    return (ExprList*)appendToList(list, (void*)value);
}
ExprList* prependExpr(ExprList* list, Expr* value) 
{
    return (ExprList*)prependToList(list, (void*)value);    
}

Expr* getExpr(ExprList* list) 
{
    return (Expr*)list->Value;
}

Expr* ast_variable(char* varName)
{
 	Expr* node = (Expr*) malloc(sizeof(Expr));
	node->kind = E_VARIABLE;
	node->attr.variable = varName;//makeVariable(varName, NULL);
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
	printPadding(level, lastChild);
	printf("E_VARIABLE\n");
	printKeyValue("NAME", expr->attr.variable, level + 1, lastChild);
}
void print_operation(Expr* expr, int level, int lastChild)
{
	printPadding(level + 1, 0);
	printf("E_OPERATION\n");
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
void print_bool(Expr* expr, int level, int lastChild)
{
	printPadding(level, lastChild);
	printf("E_BOOL\n");
	printKeyValue("VALUE", expr->attr.value == 1 ? "TRUE" : "FALSE", level + 1, 1);
}

void printExpr(Expr* expr, int level, int lastChild)
{
	switch(expr->kind)
	{
		case E_VARIABLE:
			print_variable(expr, level, lastChild);
			break;
		case E_OPERATION:
			print_operation(expr, level, lastChild);
			break;
		case E_INTEGER:
			print_integer(expr, level, lastChild);
			break;
		case E_BOOL:
			print_bool(expr, level, lastChild);
			break;
	}
}


void printExprList(ExprList* Exprlist, int level, int lastChild)
{
    printPadding(level, lastChild);
    printf("ExprList\n");
    while(Exprlist != NULL)
    {
        printExpr(getExpr(Exprlist), level + 1, IS_EMPTY_LIST(Exprlist->Next));
    }
}
