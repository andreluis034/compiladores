#include <stdio.h>
#include "mips.h"
#include <string.h>

#define SYMBOL_STR(NUM) (instruction->p##NUM->symbol.str)
#define SYMBOL_INT(NUM) (instruction->p##NUM->symbol.number)
#define SYMBOL_IS_INT(NUM) (instruction->p##NUM->type == S_INT)
#define SYMBOL_IS_STR(NUM) (instruction->p##NUM->type == S_STR)

makeTypeList(VariableList*, makeVariableList, char*)
appendType(VariableList*, appendVariable, char*)
prependType(VariableList*, prependVariable, char*)
getType(char*, getVariable, VariableList*)

VariableList* globalVariables;

/** Might be useful
* .macro push (%register)
sub $sp,$sp,4
sw %register,($sp)
.end_macro
.macro pop (%register)
lw %register,($sp)
addiu $sp,$sp,4
.end_macro
*/

int existsVariable(VariableList* list, char* toFind){
    while(list!=NULL){
        if (strcmp(getVariable(list),toFind) == 0){
            return 1;
        }
        list = list->Next;
    }

    return 0;
}

void printVariableList(VariableList* list){
    printf(".data\n");
    while(list!=NULL){
        printf("%s: .word 0\n",getVariable(list));
        list = list->Next;
    }
}

void printSimpleOperation(Inst* instruction, char* addorsub){
    
    if(addorsub[0]=='a' || addorsub[0]=='s'){
        if(SYMBOL_IS_INT(2) && SYMBOL_IS_INT(3))
        {
            printf("    li %s %d\n",SYMBOL_STR(1),SYMBOL_INT(2));
            printf("    %si %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(1),SYMBOL_INT(3));
        }
        else
        {
            if(SYMBOL_IS_STR(2) && SYMBOL_IS_INT(3)){
                printf("    %si %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_INT(3));
            }
            
            else if (SYMBOL_IS_INT(2) && SYMBOL_IS_STR(3))
            {
                printf("    %si %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(3),SYMBOL_INT(2));
            }
            else{
                printf("    %s %s %s %s\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
            }
        }  
    }
    
    if(addorsub[0]=='m' || addorsub[0]=='d'){
        if(SYMBOL_IS_INT(2) && SYMBOL_IS_INT(3))
        {
            printf("    li %s %d\n",SYMBOL_STR(1),SYMBOL_INT(2));
            printf("    %s %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(1),SYMBOL_INT(3));
        }
        else
        {
            if(SYMBOL_IS_STR(2) && SYMBOL_IS_INT(3)){
                printf("    %s %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_INT(3));
            }
            
            else if (SYMBOL_IS_INT(2) && SYMBOL_IS_STR(3))
            {
                printf("    %s %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(3),SYMBOL_INT(2));
            }
            else{
                printf("    %s %s %s %s\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
            }
        }  
    }
    
    
}

void printStore(Inst* instruction){

    //x:=2;
    //SYMBOL 1: x variavel onde guardar
    //SYMBOL 2: 2 resultado

    if(SYMBOL_IS_STR(2)){
        printf("    la $t8 %s\n",SYMBOL_STR(1));
        printf("    sw %s 0($t8)\n",SYMBOL_STR(2));
    }
    else{
        printf("    la $t8 %s\n",SYMBOL_STR(1));
        printf("    li $t9 %d\n",SYMBOL_INT(2));
        printf("    sw $t9 0($t8)\n");
    }

    //printf("    la %s %d\n",SYMBOL_STR(1),SYMBOL_INT(2));
}

void printCompare(Inst* instruction, char* comparation){

    //printf("        TESTING... %s %d %d\n",SYMBOL_STR(1),SYMBOL_INT(2),SYMBOL_INT(3));

    if(SYMBOL_IS_STR(2) && SYMBOL_IS_STR(3))
    {
        printf("    %s %s %s %s\n",comparation,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
        //printf("teste: %s %s %s\n",SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
    }

    else if(SYMBOL_IS_STR(2) && SYMBOL_IS_INT(3))
    {   
        printf("    li $t9 %d\n",SYMBOL_INT(3));
        printf("    %s %s %s $t9\n",comparation,SYMBOL_STR(1),SYMBOL_STR(2));
    }

    else if(SYMBOL_IS_INT(2) && SYMBOL_IS_STR(3)){
        printf("    li $t9 %d\n",SYMBOL_INT(2));
        printf("    %s %s %s $t9\n",comparation,SYMBOL_STR(1),SYMBOL_STR(3));
    }

    else{
        //two ints, gotta load one into temp reg
        printf("    li $t9 %d\n",SYMBOL_INT(2));
        printf("    li $t8 %d\n",SYMBOL_INT(3));
        printf("    %s %s $t8 $t9\n",comparation,SYMBOL_STR(1));
    }

    //printf("cenas: %s %d %d\n",SYMBOL_STR(1),SYMBOL_INT(2),SYMBOL_INT(3));
    
}

void compileSingleInstruction(Inst* instruction)
{
    
    
    switch(instruction->type)
    {
        case LABEL:
        printf("%s:\n", instruction->p1->symbol.str);
        break;
        case ADD:
        printSimpleOperation(instruction,"add");
        break;
        case SUB:
        printSimpleOperation(instruction,"sub");
        break;
        case MUL:
        printSimpleOperation(instruction,"mul");
        break;
        case DIV:
        printSimpleOperation(instruction,"div");
        break;
        case EQL:
        printCompare(instruction,"seq");
        break;
        case LESS:
        printCompare(instruction,"slt");
        break;
        case LESSEQ:
        printCompare(instruction,"sle");
        break;
        case MORE:
        printCompare(instruction,"sgt");
        break;
        case MOREEQ:
        printCompare(instruction,"sge");
        break;
        case OR:
        printCompare(instruction,"or");
        break;
        case AND:
        printCompare(instruction,"and");
        break;
        case GOTO:
        printf("    j %s\n",SYMBOL_STR(1));
        break;
        case RETURN://TODO return values
        printf("    jr $ra\n");
        break;
        case BRANCH_EQ_ZERO:
        printf("    beqz %s %s\n",SYMBOL_STR(1),SYMBOL_STR(2));
        break;
        case BRANCH_NOT_EQ_ZERO:
        printf("    bneq %s %s\n",SYMBOL_STR(1),SYMBOL_STR(2));
        break;
        case LOAD_VARIABLE: //TODO
        printf("    lw %s %s\n",SYMBOL_STR(1),SYMBOL_STR(2));
        break;
        case STORE_VARIABLE:
        printStore(instruction);
        break;

    }
}




void checkDeclaration(Cmd* cmd)  
{   
    //printf("%s\n",cmd->attr.declaration.variable->attr.variable);
    if(existsVariable(globalVariables,cmd->attr.declaration.variable->attr.variable)){
    }
    else{
        globalVariables = appendVariable(globalVariables,cmd->attr.declaration.variable->attr.variable);
    }
    
}


void checkIfElse(Cmd* cmd)  
{
    int lastChild = IS_EMPTY_LIST( cmd->attr.ifelse.iffalse );
    checkCmdList(cmd->attr.ifelse.iftrue);
    if(!lastChild)
    {
        checkCmdList(cmd->attr.ifelse.iffalse);
    }
}

void checkFor(Cmd* cmd) 
{

    if(cmd->attr.forCmd.initial != NULL)
    {
        checkCmd(cmd->attr.forCmd.initial);
    }

    if(cmd->attr.forCmd.afterIteration != NULL)
    {
        checkCmd(cmd->attr.forCmd.afterIteration);
    }

    checkCmdList(cmd->attr.forCmd.body);

}

void checkFunc(Cmd* cmd)
{
    //printExprList(cmd->attr.func.argList, level + 1, 0);

    //check args of function

    ExprList* argList = cmd->attr.func.argList;

    while(argList != NULL)
    {
        globalVariables = appendVariable(globalVariables,getExpr(argList)->attr.variable);
        //printf("%s\n",getExpr(argList)->attr.variable);
        argList = argList->Next;
    }

    checkCmdList(cmd->attr.func.commandList);
}

void checkCmd(Cmd* cmd)  
{
    switch(cmd->kind)
    {
        case C_FUNC: 
            checkFunc(cmd);
        break;

        case C_IF_ELSE:
            checkIfElse(cmd);
        break;

        case C_FOR:
            checkFor(cmd);
        break;

        case C_DECLARATION:
            checkDeclaration(cmd);
        break;
    }
}

void checkCmdList(CmdList* cmdlist)
{
    while(cmdlist != NULL)
    {
        checkCmd(getCmd(cmdlist));
        cmdlist = cmdlist->Next;
    }
}







void compileToMips(InstList* instructionList, CmdList* cmdlist) 
{
    globalVariables = NULL;
    checkCmdList(cmdlist);
    printVariableList(globalVariables);
    printf(".text\n");
    //print function
    //printf("PRINT:\n");
    //scan function
    //printf("SCAN:\n");
    //main starts here
    printf("    jal main\n");
    printf("    li $v0, 10\n");
    printf("    syscall\n");
    while(instructionList != NULL) 
    {
        Inst* inst = (Inst*) instructionList->Value.pointer;
        compileSingleInstruction(inst);
        instructionList = instructionList->Next;
    }
    
}