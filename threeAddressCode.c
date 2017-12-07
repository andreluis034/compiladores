#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "threeAddressCode.h"
#define GET_T (tCount++)
#define GET_LABEL (labelCount++)
#define REGISTER_COUNT 32
#define ARG_REGISTER_START 4
#define STACK_POINTER 29
#define FRAME_POINTER 30
#define RETURN_ADDRESS 31
unsigned int tCount = 0;
unsigned int labelCount = 0;
Register registers[REGISTER_COUNT] =  {0};
char* registerNames[] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7", "$t8", "$t9","$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

makeTypeList(InstList*, makeInstList, Inst*)
appendType(InstList*, appendInst, Inst*)
prependType(InstList*, prependInst, Inst*)
getType(Inst*, getInst, InstList*)
concatType(InstList*, concatInst)

void initializeRegisters() 
{
	int i;
	for(i = 0; i < REGISTER_COUNT; ++i) 
	{
		registers[i].registerName = registerNames[i];
		registers[i].registerNumber = i;
		registers[i].used = (i <= 7 || i >= 24) ? 1 : 0;
		registers[i].variableRepresented = NULL;
	}
}

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

int getFreeRegister()
{
	int i;
	int ret = -1;
	for(i = 0; i < REGISTER_COUNT; ++i)
	{
		if(registers[i].used == 0)
		{
			if(registers[i].variableRepresented == NULL)
			{
				ret = i;
				break;
			}
			ret = i;
		}
		
	}
	registers[ret].variableRepresented = NULL;
	registers[i].used = 1;
	return ret;
}

int getFreeRegisterForVariable(char* varName, int* reused) {
	int i;
	int ret = -1;
	*reused = 0;
	for(i = 0; i < REGISTER_COUNT; ++i)
	{
		
		if(registers[i].variableRepresented != NULL && strcmp(varName, registers[i].variableRepresented) == 0)
		{
			ret = i; 
			*reused = 1;
			break;
		}
		if(registers[i].used == 0) 
		{
			ret = i;
			if(registers[i].variableRepresented == NULL)
			{
				break;
			}
		}
	}
	registers[ret].variableRepresented = varName;
	registers[ret].used = 1;
	return ret;
}

//Declare that the register can be used for something else but we can still re use its value later
void freeRegister(InstSymbol* symb) 
{
	int i;
	if(symb->type == S_INT) 
	{
		printf("#TRYING TO FREE CONSTANT %d\n", symb->symbol.number);
		exit(-1);
		return;
	}
	for(i = 0; i < REGISTER_COUNT; ++i)
	{
		if(strcmp(symb->symbol.str, registers[i].registerName) == 0)
		{
			registers[i].used = 0;
			return;
		}
	}
}

InstSymbol* getNextSymbol(int reg) 
{
	//int reg = getRegisterForVariable(); //getUnusedRegister();
	
	if(reg == -1) {
		printf("#COULD NOT GET ENOUGH REGISTERS\n");
		exit(-1);
	}
	return makeInstSymbolStr(registers[reg].registerName);
	
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
//NAKANAIDE IMA WA
Pair* CompileExpression(char* operator, Pair* left, Pair* right) 
{
	if(left->symbol->type == S_STR)
	freeRegister(left->symbol);
	if(right->symbol->type == S_STR)
	freeRegister(right->symbol);
	InstSymbol* nextSymbol = getNextSymbol(getFreeRegister());//TODO: use one of the expressions
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
	InstList* instList  = NULL;
	while(cmdlist != NULL) {
		instList = concatInst(instList, compileCommand(getCmd(cmdlist)));
		cmdlist = cmdlist->Next;
	}
	return instList; 
	
}

Pair* loadVariable(char* varName) 
{
	int reuse = 0;
	Inst* inst = NULL;
	InstList* lw = EMPTY_LIST;
	int reg = getFreeRegisterForVariable(varName, &reuse);
	InstSymbol* is = getNextSymbol(reg);
	if(reuse == 0) 
	{
		inst = makeInstruction(LOAD_VARIABLE, is, makeInstSymbolStr(varName), NULL);
	}
	lw = prependInst(lw, inst);
	return makePair(is, lw);
}

Pair* loadVariableAddress(char* varName)
{

} 

InstList* compileCommand(Cmd* cmd) 
{
	ExprList* exprlist;
	InstList* instructionList = EMPTY_LIST;
	Pair* compiledExpr;
	Pair* var;
	int regCount = 0;
	InstSymbol* symbol;
	InstSymbol* symbol2;
	Inst* compiledInst;
	InstSymbol* exitif;
	char op[2];
	switch(cmd->kind) 
	{
		case C_DECLARATION:
			compiledExpr = makePairExpr(cmd->attr.declaration.expr);
			instructionList = compiledExpr->instructionList;
			symbol = getNextSymbol(getFreeRegister());
			compiledInst = makeInstruction(LOAD_ADDRESS, symbol, makeInstSymbolStr(cmd->attr.declaration.variable->attr.variable), NULL);
			instructionList = appendInst(instructionList, compiledInst);
			compiledInst = makeInstruction(STORE_VARIABLE, compiledExpr->symbol, symbol, makeInstSymbolInt(0));
			instructionList = appendInst(instructionList, compiledInst);
			freeRegister(symbol);
			if(compiledExpr->symbol->type == S_STR)
				freeRegister(compiledExpr->symbol);
			break;
			
		case C_INCREMENT:
			var = makePairExpr(cmd->attr.increment.variable);
			//compiledExpr = makePairExpr(cmd->attr.increment.)
			if(strcmp(cmd->attr.increment.operator, "++") == 0 || strcmp(cmd->attr.increment.operator, "--") == 0) 
			{
				compiledExpr = makePairInt(1, NULL);
			}
			else 
			{
				compiledExpr = makePairExpr(cmd->attr.increment.expr);
			}
			op[0] = cmd->attr.increment.operator[0];
			op[1] = 0;
			compiledExpr = CompileExpression(op, var, compiledExpr);
			instructionList = compiledExpr->instructionList;
			instructionList = appendInst(instructionList, 
				makeInstruction(STORE_VARIABLE, makeInstSymbolStr(cmd->attr.declaration.variable->attr.variable), 
				compiledExpr->symbol, NULL));
			if(compiledExpr->symbol->type == S_STR)
				freeRegister(compiledExpr->symbol);		
			//appendInst(makeInstruction(ADD,))
			break;
				
		case C_FOR:
			//inst inicial
			instructionList = compileCommand(cmd->attr.forCmd.initial);
			//instructionList = appendInst(instructionList,compiledInst);
			//LABEL
			symbol = getLabel();
			instructionList = appendInst(instructionList,makeInstruction(LABEL,symbol,NULL,NULL));
			//TEST
			compiledExpr = makePairExpr(cmd->attr.forCmd.condition);
			instructionList = concatList(instructionList,compiledExpr->instructionList);
			//BRANCH_EQ_ZERO
			exitif = getLabel();
			instructionList = appendInst(instructionList, makeInstruction(BRANCH_EQ_ZERO, compiledExpr->symbol, exitif, NULL));
			freeRegister(compiledExpr->symbol);
			//CMDLIST
			instructionList = concatInst(instructionList,compileCmdList(cmd->attr.forCmd.body));
			//INST
			instructionList = concatInst(instructionList,compileCommand(cmd->attr.forCmd.afterIteration));
			//GOTO LABEL
			instructionList = appendInst(instructionList,makeInstruction(GOTO,symbol,NULL,NULL));
			//LABEL1
			instructionList = appendInst(instructionList,makeInstruction(LABEL,exitif,NULL,NULL));
		break;
			
		case C_IF_ELSE: 
			//Compile expression
			compiledExpr = makePairExpr(cmd->attr.ifelse.condition);
			instructionList = compiledExpr->instructionList;
			symbol = getLabel();
			exitif = getLabel();
			//TEST
			instructionList = appendInst(instructionList, makeInstruction(BRANCH_EQ_ZERO, compiledExpr->symbol, symbol, NULL));
			freeRegister(compiledExpr->symbol);
			//IFTRUE
			instructionList = concatInst(instructionList, compileCmdList(cmd->attr.ifelse.iftrue));
			instructionList = appendInst(instructionList, makeInstruction(GOTO, exitif, NULL, NULL)); //EXIT
			//IFFALSE
			instructionList = appendInst(instructionList, makeInstruction(LABEL, symbol, NULL, NULL));
			instructionList = concatInst(instructionList, compileCmdList(cmd->attr.ifelse.iffalse));
			//FINISH
			instructionList = appendInst(instructionList, makeInstruction(LABEL, exitif, NULL, NULL));
		break;
			
		case C_FUNC: //TODO arguments
			symbol = getNextSymbol(STACK_POINTER);
			symbol2 = getNextSymbol(FRAME_POINTER);
			compiledInst = makeInstruction(LABEL, makeInstSymbolStr(cmd->attr.func.funcName), NULL, NULL);
			//STACK ALLOCATION
			instructionList = appendInst(instructionList, compiledInst);
			//addi $sp $sp -4
			compiledInst = makeInstruction(ADD, symbol, symbol, makeInstSymbolInt(-4));
			instructionList = appendInst(instructionList, compiledInst);
			//sw $fp 0($sp)
			compiledInst = makeInstruction(STORE_VARIABLE, symbol2, symbol, makeInstSymbolInt(0));
			instructionList = appendInst(instructionList, compiledInst);
			//addi $fp $sp 4 
			compiledInst = makeInstruction(ADD, symbol2, symbol, makeInstSymbolInt(4));
			instructionList = appendInst(instructionList, compiledInst);
			//addi $sp $sp -(scope_size - 4)
			compiledInst = makeInstruction(ADD, symbol, symbol, makeInstSymbolInt(-cmd->attr.func.scope->scope_size + 4));
			instructionList = appendInst(instructionList, compiledInst);
			//sw $ra -8($fp)
			compiledInst = makeInstruction(STORE_VARIABLE, getNextSymbol(RETURN_ADDRESS), symbol2, makeInstSymbolInt(-8));
			instructionList = appendInst(instructionList, compiledInst);

			//Normal function operation
			instructionList = concatInst(instructionList, compileCmdList(cmd->attr.func.commandList));
			instructionList = prependInst(instructionList, compiledInst);

			//addi $sp $sp total_size
			compiledInst = makeInstruction(ADD, symbol, symbol, makeInstSymbolInt(cmd->attr.func.scope->total_size));
			instructionList = appendInst(instructionList, compiledInst);
			//lw $ra -8($fp)
			compiledInst = makeInstruction(LOAD_VARIABLE, getNextSymbol(RETURN_ADDRESS), symbol2, makeInstSymbolInt(-8));
			instructionList = appendInst(instructionList, compiledInst);
			//lw $fp -8($fp)
			compiledInst = makeInstruction(LOAD_VARIABLE, symbol2, symbol2, makeInstSymbolInt(-4));
			instructionList = appendInst(instructionList, compiledInst);

			instructionList = appendInst(instructionList, makeInstruction(RETURN, NULL, NULL, NULL));
			//TODO jal?
		break;
		case C_FUNC_CALL:
			//TODO save used registers
			symbol = getNextSymbol(getFreeRegister());
			if(strcmp(cmd->attr.funcCall.funcName, "fmt.Scan") == 0) 
			{
				compiledInst = makeInstruction(LOAD_ADDRESS, symbol, makeInstSymbolStr(getExpr(cmd->attr.funcCall.variables)->attr.variable), NULL);
				instructionList = appendInst(instructionList, compiledInst);
				compiledInst = makeInstruction(LOAD_ARGUMENT_REGISTER, makeInstSymbolStr("$a0"), symbol, NULL);
				instructionList = appendInst(instructionList, compiledInst);
				freeRegister(symbol);
			}
			else 
			{
				exprlist = cmd->attr.funcCall.variables;
				while(exprlist != NULL) {
					compiledExpr = makePairExpr(getExpr(exprlist));
					if(regCount < ARG_REGISTER_COUNT)
					{
						compiledInst = makeInstruction(LOAD_ARGUMENT_REGISTER, getNextSymbol(regCount + ARG_REGISTER_START), compiledExpr->symbol, NULL);
						regCount++;
					}
					else
					{
						compiledInst = makeInstruction(LOAD_ARGUMENT_STACK, compiledExpr->symbol, NULL, NULL);
					}
					instructionList = appendInst(compiledExpr->instructionList, compiledInst);
					freeRegister(compiledExpr->symbol);
					exprlist = exprlist->Next;
				}
			}
			compiledInst = makeInstruction(FUNC_CALL, makeInstSymbolStr(cmd->attr.funcCall.funcName), NULL, NULL );
			instructionList = appendInst(instructionList, compiledInst);
			
		break;
	}
	return instructionList;
}
		
		
		
/*      E_INTEGER,
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
        return loadVariable(expr->attr.variable);// makePairStr(expr->attr.variable, NULL);
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