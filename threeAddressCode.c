#include <string.h>
#include "threeAddressCode.h"
#define GET_T (tCount++)
int tCount = 0;

Pair* makePairInt(int i, InstList* list)
{
	Pair* p = malloc(sizeof(Pair));
	p->first.number = i;
	p->instructionList = list;
	return p;
}

Pair* makePairStr(char* str, InstList* list) 
{
	Pair* p = malloc(sizeof(Pair));
	p->first.str = str;
	p->instructionList = list;
	return p;	
}


/*		E_INTEGER,
		E_OPERATION,
		E_VARIABLE,*/
Pair* compileExpression(Expr* expr) 
{
	Pair* p;
	switch(expr->kind) 
	{
		case E_INTEGER:
			return makePairInt(expr->attr.value, NULL);
		case E_VARIABLE: 
			return makePairStr(expr->attr.variable, NULL);
		case E_OPERATION: 
			switch(expr->attr.op.operator[0])
			{
				case '*':
					break;
			}
			break;
	}	
	return p;
}