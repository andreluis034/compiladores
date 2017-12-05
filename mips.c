#include <stdio.h>
#include "mips.h"

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
            printf("li %s %d\n",SYMBOL_STR(1),SYMBOL_INT(2));
            printf("%si %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(1),SYMBOL_INT(3));
        }
        else
        {
            if(SYMBOL_IS_STR(2) && SYMBOL_IS_INT(3)){
                printf("%si %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_INT(3));
            }
            
            else if (SYMBOL_IS_INT(2) && SYMBOL_IS_STR(3))
            {
                printf("%si %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(3),SYMBOL_INT(2));
            }
            else{
                printf("%s %s %s %s\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
            }
        }  
    }
    
    if(addorsub[0]=='m' || addorsub[0]=='d'){
        if(SYMBOL_IS_INT(2) && SYMBOL_IS_INT(3))
        {
            printf("li %s %d\n",SYMBOL_STR(1),SYMBOL_INT(2));
            printf("%s %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(1),SYMBOL_INT(3));
        }
        else
        {
            if(SYMBOL_IS_STR(2) && SYMBOL_IS_INT(3)){
                printf("%s %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_INT(3));
            }
            
            else if (SYMBOL_IS_INT(2) && SYMBOL_IS_STR(3))
            {
                printf("%s %s %s %d\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(3),SYMBOL_INT(2));
            }
            else{
                printf("%s %s %s %s\n",addorsub,SYMBOL_STR(1),SYMBOL_STR(2),SYMBOL_STR(3));
            }
        }  
    }
    
    
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
        case GOTO:
        printf("j %s\n",SYMBOL_STR(1));
        break;
        case RETURN://TODO return values
        printf("jr $ra\n");
        break;
    }
}

void compileToMips(InstList* instructionList) 
{
    printf(".text\n");
    printf("jal main\n");
    printf("li $v0, 10\n");
    printf("syscall\n");
    while(instructionList != NULL) 
    {
        Inst* inst = (Inst*) instructionList->Value.pointer;
        compileSingleInstruction(inst);
        instructionList = instructionList->Next;
    }
}