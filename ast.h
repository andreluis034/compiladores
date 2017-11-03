
// AST definitions
#ifndef __ast_h__
#define __ast_h__

#include "list.h"
typedef List ExprList;

// AST for expressions
struct _Expr {
	enum { 
		E_INTEGER,
		E_OPERATION,
		E_VARIABLE,
		E_BOOL
	} kind;
	union {
		int value; // for integer values
		struct { 
			char* operator; // PLUS, MINUS, etc 
			struct _Expr* left;
			struct _Expr* right;
		} op; // for binary expressions
		char* variable;
	} attr;
};

typedef struct _Expr Expr; // Convenience typedef

ExprList* makeExprList(Expr* firstExpr);
ExprList* appendExpr(ExprList* list, Expr* value);
ExprList* prependExpr(ExprList* list, Expr* value);
Expr* getExpr(ExprList* list);
// Constructor functions (see implementation in ast.c)
Expr* ast_integer(int v);
Expr* ast_variable(char* variable);
Expr* ast_operation(char* operator, Expr* left, Expr* right);
Expr* ast_bool(int v);
void printExpr(Expr* expr, int level, int lastChild);
#endif
