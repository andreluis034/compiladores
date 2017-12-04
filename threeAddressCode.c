#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "threeAddressCode.h"
#define GET_T (tCount++)
#define GET_LABEL (labelCount++)
unsigned int tCount = 0;
unsigned int labelCount = 0;

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
InstSymbol* makeInstSymbolInt(int integer)
{
	InstSymbol* i = (InstSymbol*) malloc(sizeof(InstSymbol));
	i->type = S_INT;
	i->symbol.number = integer;
	return i;
}

InstSymbol* getNextSymbol() 
{
	char* totallySafeBuffer = (char*) malloc(13);
	snprintf(totallySafeBuffer, 13, "t%d", GET_T);
	return makeInstSymbolStr(totallySafeBuffer);
}
InstSymbol* getLabel() 
{
	char* totallySafeBuffer = (char*) malloc(15);
	snprintf(totallySafeBuffer, 15, "LBL%d", GET_LABEL);
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
#define COMPILE_OPERATOR(ifORElseIF, op, enum) ifORElseIF(strcmp(operator, op) == 0) {\
	Inst* inst = makeInstruction(enum, nextSymbol, left->symbol, right->symbol);\
	requiredInstructions = appendInst(requiredInstructions, inst);\
} 
//\<|\<\=|\>|\>\=
//NAKANAIDE IMA WA
Pair* CompileExpression(char* operator, Pair* left, Pair* right) 
{
	InstSymbol* nextSymbol = getNextSymbol();//TODO: use one of the expressions
	InstList* requiredInstructions = concatInst(left->instructionList, right->instructionList); 
	COMPILE_OPERATOR(if, "*", MUL)
	COMPILE_OPERATOR(else if, "/", DIV)
	COMPILE_OPERATOR(else if, "+", ADD)
	COMPILE_OPERATOR(else if, "-", SUB)
	COMPILE_OPERATOR(else if, "==", EQL)
	COMPILE_OPERATOR(else if, "||", OR)
	COMPILE_OPERATOR(else if, "&&", AND)
	COMPILE_OPERATOR(else if, "<",LESS)
	COMPILE_OPERATOR(else if, "<=",LESSEQ)
	COMPILE_OPERATOR(else if, ">",MORE)
	COMPILE_OPERATOR(else if, ">=",MOREEQ)

	return makePair(nextSymbol, requiredInstructions);
}

InstList* compileCmdList(CmdList* cmdlist) 
{
	return NULL; //TODO
}
/*		C_DECLARATION,
		C_INCREMENT,
		C_IF_ELSE,
		C_FOR,
		C_FUNC_CALL,
		C_FUNC*/
InstList* compileCommand(Cmd* cmd) 
{
	InstList* instructionList = EMPTY_LIST;
	Pair* compiledExpr;
	InstSymbol* symbol;
	Inst* compiledInst;
	InstSymbol* exitif;
	switch(cmd->kind) 
	{
		case C_DECLARATION:
			compiledExpr = makePairExpr(cmd->attr.declaration.expr);
			instructionList = compiledExpr->instructionList;
			instructionList = appendInst(instructionList, 
				makeInstruction(MOV, makeInstSymbolStr(cmd->attr.declaration.variable), 
				compiledExpr->symbol, NULL));
			break;
		case C_INCREMENT:
			//appendInst(makeInstruction(ADD,))
			break;
		case C_FOR:
			//inst inicial
			compiledInst = compileCommand(cmd->attr.forCmd.initial);
			instructionList = appendInst(instructionList,compiledInst);
			//LABEL
			symbol = getLabel();
			instructionList = appendInst(instructionList,makeInstruction(LABEL,symbol,NULL,NULL));
			//TEST
			compiledExpr = makePairExpr(cmd->attr.forCmd.condition);
			instructionList = concatList(instructionList,compiledExpr->instructionList);
			//BRANCH_EQ_ZERO
			exitif = getLabel();
			instructionList = appendInst(instructionList, makeInstruction(BRANCH_EQ_ZERO, compiledExpr->symbol, exitif, NULL));
			//CMDLIST
			instructionList = concatList(instructionList,compileCmdList(cmd->attr.forCmd.body));
			//INST
			compiledInst = compileCommand(cmd->attr.forCmd.afterIteration);
			instructionList = appendInst(instructionList,compiledInst);
			//GOTO LABEL
			instructionList = appendInst(instructionList,makeInstruction(GOTO,symbol,NULL,NULL));
			//LABEL1
			instructionList = appendInst(instructionList,makeInstruction(LABEL,exitif,NULL,NULL));
			break;
		case C_IF_ELSE: 
			compiledExpr = makePairExpr(cmd->attr.ifelse.condition);
			instructionList = compiledExpr->instructionList;
			symbol = getLabel();
			exitif = getLabel();
			instructionList = appendInst(instructionList, makeInstruction(BRANCH_EQ_ZERO, compiledExpr->symbol, symbol, NULL));
			instructionList = concatInst(instructionList, compileCmdList(cmd->attr.ifelse.iftrue));
			instructionList = appendInst(instructionList, makeInstruction(GOTO, exitif, NULL, NULL));
			instructionList = appendInst(instructionList, makeInstruction(LABEL, symbol, NULL, NULL));
			instructionList = concatInst(instructionList, compileCmdList(cmd->attr.ifelse.iffalse));
			instructionList = appendInst(instructionList, makeInstruction(LABEL, exitif, NULL, NULL));
			break;
		case C_FUNC: //TODO arguments
			compiledInst = makeInstruction(LABEL, makeInstSymbolStr(cmd->attr.func.funcName), NULL, NULL);
			instructionList = compileCmdList(cmd->attr.func.commandList);
			instructionList = prependInst(instructionList, compiledInst);
			//TODO jal?
			break;
	}
	return instructionList;
}

/*		E_INTEGER,
		E_OPERATION,
		E_VARIABLE,*/
Pair* makePairExpr(Expr* expr) 
{
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