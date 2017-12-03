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
	i->str = str;
	return i;
}
InstSymbol* makeInstSymbolInt(int intlel)
{
	InstSymbol* i = (InstSymbol*) malloc(sizeof(InstSymbol));
	i->number = intlel;
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
	InstSymbol* symbol = malloc(sizeof(InstSymbol));
	symbol->number = i;
	return makePair(symbol, list);
}

Pair* makePairStr(char* str, InstList* list) 
{
	InstSymbol* symbol = malloc(sizeof(InstSymbol));
	symbol->str = str;
	return makePair(symbol, list);
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
//NAKANAIDE IMA WA
Pair* CompileExpression(char* operator, Pair* left, Pair* right) 
{
	InstSymbol* nextSymbol = getNextSymbol();
	InstList* requiredInstructions = concatInst(left->instructionList, left->instructionList); 
	if(strcmp(operator, "*") == 0) 
	{
		Inst* inst = makeInstruction(MUL, nextSymbol, left->symbol, right->symbol);
		requiredInstructions = appendInst(requiredInstructions, inst);
	}
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