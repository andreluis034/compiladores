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
#define RETURN_REGISTER 2
#define GENERIC_REGISTER 8
unsigned int tCount = 0;
unsigned int labelCount = 0;
Register registers[REGISTER_COUNT] =  {0};
Register registers_backup[REGISTER_COUNT] =  {0};
char* registerNames[] = {"$zero","$at","$v0","$v1","$a0","$a1","$a2","$a3","$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$s0","$s1","$s2","$s3","$s4","$s5","$s6","$s7", "$t8", "$t9","$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};
Scope* currentScope;
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
	for(i = GENERIC_REGISTER; i < REGISTER_COUNT; ++i)
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
	for(i = GENERIC_REGISTER; i < REGISTER_COUNT; ++i)
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
	if(symb->symbol.str[1] == 'z' || symb->symbol.str[1] == 'a' || symb->symbol.str[1] == 'v')
	{
		return;
	}
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
	Variable* variable = getVariableScope(currentScope, varName);
	if(variable == NULL)
	{
		printf("%s variable not found in the current scope\n", varName);
		exit(-1);
	}	
	if(variable->location == registerr)
	{
		return makePair(getNextSymbol(variable->position.registerNumber + ARG_REGISTER_START), lw);
	}
	int reg = getFreeRegisterForVariable(varName, &reuse);
	InstSymbol* is = getNextSymbol(reg);
	if(reuse == 0) 
	{
		inst = makeInstruction(LOAD_VARIABLE, is, makeInstSymbolStr("$fp"), makeInstSymbolInt( variable->position.stackOffset));
	}
	lw = prependInst(lw, inst);
	return makePair(is, lw);
}

InstList* returnFunction()
{
	InstList* instructionList = EMPTY_LIST;
	Inst* compiledInst;
	InstSymbol* symbol = getNextSymbol(STACK_POINTER);
	InstSymbol* symbol2 = getNextSymbol(FRAME_POINTER);
	//addi $sp $sp total_size
	compiledInst = makeInstruction(ADD, symbol, symbol, makeInstSymbolInt(currentScope->scope_size));
	instructionList = appendInst(instructionList, compiledInst);
	//lw $ra -8($fp)
	compiledInst = makeInstruction(LOAD_VARIABLE, getNextSymbol(RETURN_ADDRESS), symbol2, makeInstSymbolInt(-8));
	instructionList = appendInst(instructionList, compiledInst);
	//lw $fp -8($fp)
	compiledInst = makeInstruction(LOAD_VARIABLE, symbol2, symbol2, makeInstSymbolInt(-4));
	instructionList = appendInst(instructionList, compiledInst);
	instructionList = appendInst(instructionList, makeInstruction(RETURN, NULL, NULL, NULL));
	return instructionList;
}
InstList* activationFunctionRecord(Cmd* cmd)
{
	InstList* instructionList = EMPTY_LIST;
	Inst* compiledInst;
	InstSymbol* symbol = getNextSymbol(STACK_POINTER);
	InstSymbol* symbol2 = getNextSymbol(FRAME_POINTER);
	currentScope = cmd->attr.func.scope;
	
	compiledInst = makeInstruction(LABEL, makeInstSymbolStr(cmd->attr.func.funcName), NULL, NULL);
	instructionList = appendInst(instructionList, compiledInst);
	//STACK ALLOCATION
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
	return instructionList;
}

InstList* storeCompiledExpression(Pair* expr, char* varName) //TODO update register
{
	Inst* compiledInst = NULL;
	InstList* instructionList = expr->instructionList;
	Variable* variable = getVariableScope(currentScope, varName);
	if(variable == NULL)
	{
		printf("%s variable not found in the current scope\n", varName);
		exit(-1);
	}
	if(variable->location == registerr) 
	{
		compiledInst = makeInstruction(ADD, getNextSymbol(variable->position.registerNumber + ARG_REGISTER_START), getNextSymbol(0), expr->symbol);
	}
	else
	{
		compiledInst = makeInstruction(STORE_VARIABLE, expr->symbol, getNextSymbol(FRAME_POINTER), makeInstSymbolInt(variable->position.stackOffset));
	}
	instructionList = appendInst(instructionList, compiledInst);
	return instructionList;
}
InstList* saveUsedRegisters()
{
	InstList* instructionList = EMPTY_LIST;
	int i;
	memcpy(registers_backup, registers, sizeof(registers));
	for(i = 1; i < REGISTER_COUNT; ++i)
	{
		if(registers[i].used == 1 && !(i <= 3 || i >= 24))
		{
			if((i >= 4 && i <= 7 && currentScope->argument_registers + ARG_REGISTER_START - 1 >= i) || i > 7 )
			{
				instructionList = appendInst(instructionList, makeInstruction(PUSH, getNextSymbol(registers[i].registerNumber), NULL, NULL));
			} 
		}
		registers[i].used = (i <= 3 || i >= 24) ? 1 : 0;
		registers[i].variableRepresented = NULL;
	}
	return instructionList;
}

InstList* restoreRegisters()
{
	InstList* instructionList = EMPTY_LIST;
	int i;
	memcpy(registers, registers_backup, sizeof(registers));
	for(i = (REGISTER_COUNT - 1); i >= 1; --i)
	{
		if(registers[i].used == 1 && !(i <= 3 || i >= 24))
		{
			if((i >= 4 && i <= 7 && currentScope->argument_registers + ARG_REGISTER_START - 1 >= i) || i > 7 )
			{
				instructionList = appendInst(instructionList, makeInstruction(POP, getNextSymbol(registers[i].registerNumber), NULL, NULL));
			} 
		}
		else if(!(i <= 3 || i >= 24))
		{
			registers[i].variableRepresented = NULL;
		}
	}
	return instructionList;
}
InstList* compileCommand(Cmd* cmd) 
{
	ExprList* exprlist;
	InstList* instructionList = EMPTY_LIST;
	Pair* compiledExpr;
	Pair* var;
	int regCount = 0;
	int argsPushedToStack = 0;
	InstSymbol* symbol;
	InstSymbol* symbol2;
	Inst* compiledInst;
	InstList* lastInsts = NULL;
	InstSymbol* exitif;
	Variable* variable;
	char op[2];
	switch(cmd->kind) 
	{
		case C_DECLARATION:
			compiledExpr = makePairExpr(cmd->attr.declaration.expr);
			instructionList = storeCompiledExpression(compiledExpr, cmd->attr.declaration.variable->attr.variable);

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
			instructionList = storeCompiledExpression(compiledExpr, cmd->attr.increment.variable->attr.variable);

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
			
		case C_FUNC:
			instructionList = activationFunctionRecord(cmd);
			//Normal function operation
			instructionList = concatInst(instructionList, compileCmdList(cmd->attr.func.commandList));
			instructionList = concatInst(instructionList, returnFunction());
			currentScope = NULL;
			break;
		case C_FUNC_CALL:
			//TODO save used registers
			instructionList = saveUsedRegisters();
			if(strcmp(cmd->attr.funcCall.funcName, "fmt.scan") == 0) 
			{
				lastInsts = storeCompiledExpression(makePairStr(registers[RETURN_REGISTER].registerName, EMPTY_LIST), 
						getExpr(cmd->attr.funcCall.variables)->attr.variable);
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
						argsPushedToStack++;
						compiledInst = makeInstruction(LOAD_ARGUMENT_STACK, compiledExpr->symbol, NULL, NULL);
					}
					instructionList = concatList(instructionList, compiledExpr->instructionList);
					instructionList = appendInst(instructionList, compiledInst);
					if(compiledExpr->symbol->type == S_STR)
						freeRegister(compiledExpr->symbol);
					exprlist = exprlist->Next;
				}
			}
			compiledInst = makeInstruction(FUNC_CALL, makeInstSymbolStr(cmd->attr.funcCall.funcName), NULL, NULL );
			instructionList = appendInst(instructionList, compiledInst);
			if(lastInsts != NULL)
			{
				instructionList = concatList(instructionList, lastInsts); 
			}
			if(argsPushedToStack > 0 ){
				compiledInst = makeInstruction(ADD, getNextSymbol(STACK_POINTER), getNextSymbol(STACK_POINTER), makeInstSymbolInt(argsPushedToStack * 4) );
				instructionList = appendInst(instructionList, compiledInst);
			}
			instructionList = concatList(instructionList, restoreRegisters());
			break;
		case C_RETURN:
			if(cmd->attr._return.value != NULL)
			{
				compiledExpr = makePairExpr(cmd->attr._return.value);
				instructionList = compiledExpr->instructionList;
				compiledInst = makeInstruction(ADD, getNextSymbol(RETURN_REGISTER), getNextSymbol(0), compiledExpr->symbol);
				instructionList = appendInst(instructionList, compiledInst);
				if(compiledExpr->symbol->type == S_STR)
					freeRegister(compiledExpr->symbol);
			}
			instructionList = concatInst(instructionList, returnFunction());
			break;
		case C_FUNC_RETURN:
			instructionList = compileCommand(cmd->attr.funcReturn.funcCall);
			instructionList = concatInst(instructionList, 
				storeCompiledExpression(makePairStr(registers[RETURN_REGISTER].registerName, EMPTY_LIST), 
					cmd->attr.funcReturn.variable->attr.variable));
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