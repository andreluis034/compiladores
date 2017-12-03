#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "threeAddressCode.h"
#define GET_T (tCount++)
unsigned int tCount = 0;

makeTypeList(InstList*, makeInstList, Inst*)
appendType(InstList*, appendInst, Inst*)
prependType(InstList*, prependInst, Inst*)
getType(Inst*, getInst, InstList*)
concatType(InstList*, concatInst)

InstSymbol* makeInstSymbolStr(char* str)
{
	InstSymbol* i = (InstSymbol*) malloc(sizeof(InstSymbol));
	i->type = S_STR;
	i->symbol.str = str;
	return i;
}
InstSymbol* makeInstSymbolInt(int intlel)
{
	InstSymbol* i = (InstSymbol*) malloc(sizeof(InstSymbol));
	i->type = S_INT;
	i->symbol.number = intlel;
	return i;
}

InstSymbol* getNextSymbol() 
{
	char* totallySafeBuffer = (char*) malloc(13);
	snprintf(totallySafeBuffer, 13, "t%d", GET_T);
	return makeInstSymbolStr(totallySafeBuffer);
}

Pair* makePair(InstSymbol* symbol, InstList* list) 
{
	Pair* p = malloc(sizeof(Pair));
	p->symbol = symbol;
	p->instructionList = list;
	return p;	
}

Pair* makePairInt(int i, InstList* list)
{
	return makePair(makeInstSymbolInt(i), list);
}

Pair* makePairStr(char* str, InstList* list) 
{
	return makePair(makeInstSymbolStr(str), list);
}

Inst* makeInstruction(InstType type, InstSymbol* s1, InstSymbol* s2, InstSymbol* s3)
{
	Inst* inst = malloc(sizeof(Inst));
	inst->type = type;
	inst->p1 = s1;
	inst->p2 = s2;
	inst->p3 = s3;
	return inst;

}
#define compileOperator(ifORElseIF, op, enum) ifORElseIF(strcmp(operator, op) == 0) {\
	Inst* inst = makeInstruction(enum, nextSymbol, left->symbol, right->symbol);\
	requiredInstructions = appendInst(requiredInstructions, inst);\
} 

//NAKANAIDE IMA WA
Pair* CompileExpression(char* operator, Pair* left, Pair* right) 
{
	InstSymbol* nextSymbol = getNextSymbol();//TODO: use one of the expressions
	InstList* requiredInstructions = concatInst(left->instructionList, left->instructionList); 
	compileOperator(if, "*", MUL)
	compileOperator(else if, "/", DIV)
	compileOperator(else if, "+", ADD)
	compileOperator(else if, "-", SUB)
	compileOperator(else if, "==", EQL)
	compileOperator(else if, "||", OR)
	compileOperator(else if, "&&", AND)

	return makePair(nextSymbol, requiredInstructions);
}

/*		E_INTEGER,
		E_OPERATION,
		E_VARIABLE,*/
Pair* makePairExpr(Expr* expr) 
{
	Pair* p;
	Pair* p1;
	Pair* p2;
	switch(expr->kind) 
	{
		case E_BOOL:
		case E_INTEGER:
			return makePairInt(expr->attr.value, NULL);
		case E_VARIABLE: 
			return makePairStr(expr->attr.variable, NULL);
		case E_OPERATION: 
			p1 = makePairExpr(expr->attr.op.left);
			p2 = makePairExpr(expr->attr.op.right);
			return CompileExpression(expr->attr.op.operator, p1, p2);
		default:
			printf("YOU FORGOT SOMETHING IDIOT type: %d\n", expr->kind); //TODO REMOVE
			return NULL;
	}	
	return NULL;
}