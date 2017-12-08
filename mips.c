#include <stdio.h>
#include <string.h>
#include "mips.h"
#include "utility.h"
#define SYMBOL_STR(NUM) (instruction->p##NUM->symbol.str)
#define SYMBOL_INT(NUM) (instruction->p##NUM->symbol.number)
#define SYMBOL_IS_INT(NUM) (instruction->p##NUM->type == S_INT)
#define SYMBOL_IS_STR(NUM) (instruction->p##NUM->type == S_STR)



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

    if(SYMBOL_IS_STR(1)){
        printf("    sw %s %d(%s)\n",SYMBOL_STR(1), SYMBOL_INT(3), SYMBOL_STR(2));
    }
    else{
        printf("    li $v1 %d\n",SYMBOL_INT(1));
        printf("    sw $v1 %d(%s)\n", SYMBOL_INT(3), SYMBOL_STR(2));
    }

}

void printCompare(Inst* instruction, char* comparation){


    if(SYMBOL_IS_STR(2) && SYMBOL_IS_STR(3))
    {
        printf("    %s %s %s %s\n",comparation,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
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
        printf("    li $t9 %d\n",SYMBOL_INT(2));
        printf("    li $t8 %d\n",SYMBOL_INT(3));
        printf("    %s %s $t8 $t9\n",comparation,SYMBOL_STR(1));
    }
    
}

void compileSingleInstruction(Inst* instruction)
{
    if(instruction == NULL)
        return;
    
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
        if(instruction->p1 != NULL)
        {
            if(SYMBOL_IS_INT(1))
                printf("    addi $v0 $zero %d\n", SYMBOL_INT(1));
            else
                printf("    add $v0 $zero %s\n", SYMBOL_STR(1));
        }
        printf("    jr $ra\n");
        break;
        case BRANCH_EQ_ZERO:
        printf("    beqz %s %s\n",SYMBOL_STR(1),SYMBOL_STR(2));
        break;
        case BRANCH_NOT_EQ_ZERO:
        printf("    bneq %s %s\n",SYMBOL_STR(1),SYMBOL_STR(2));
        break;
        case LOAD_VARIABLE: //TODO
        if(SYMBOL_STR(2)[0] == '$')
            printf("    lw %s %d(%s)\n",SYMBOL_STR(1),SYMBOL_INT(3), SYMBOL_STR(2));
        else
            printf("    lw %s %s\n",SYMBOL_STR(1), SYMBOL_STR(2));
        break;
        case STORE_VARIABLE:
        printStore(instruction);
        break;
        case LOAD_ADDRESS:
            printf("    la %s %s\n", SYMBOL_STR(1), SYMBOL_STR(2));
        break;
        case LOAD_ARGUMENT_REGISTER:
            if(SYMBOL_IS_INT(2))
            {
                printf("    addi %s $zero %d\n", SYMBOL_STR(1), SYMBOL_INT(2));
            }
            else 
            {
                printf("    add %s $zero %s\n", SYMBOL_STR(1), SYMBOL_STR(2));
            }
        break;
        case PUSH:
        case LOAD_ARGUMENT_STACK:
        printf("    addi $sp $sp -4\n");
        printf("    sw %s 0($sp)\n", SYMBOL_STR(1));
            break;
        case POP:
        printf("    lw %s 0($sp)\n", SYMBOL_STR(1));
        printf("    addi $sp $sp 4\n");
        break;
        case FUNC_CALL:
            printf("    jal %s\n", SYMBOL_STR(1));
        break;
    }
}


void compileToMips(InstList* instructionList, CmdList* cmdlist) 
{
    //globalVariables = NULL;
    printf("\n.text\n");
    //for now print and scan one arg
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

    printf("\nfmt.scan:\n");
    printf("    li $v0 5\n");
    printf("    syscall\n");
    printf("    jr $ra\n");
    printf("\nfmt.print:\n");
    printf("    li $v0 1\n");
    printf("    syscall\n");
    printf("    jr $ra\n\n");
    
}