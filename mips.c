#include <stdio.h>
#include "mips.h"

#define SYMBOL_STR(NUM) (instruction->p##NUM->symbol.str)
#define SYMBOL_INT(NUM) (instruction->p##NUM->symbol.number)
#define SYMBOL_IS_INT(NUM) (instruction->p##NUM->type == S_INT)
#define SYMBOL_IS_STR(NUM) (instruction->p##NUM->type == S_STR)

void printADDSUB(Inst* instruction, char* addorsub){

    if(SYMBOL_IS_INT(2) && SYMBOL_IS_INT(3))
            {
                printf("%si %s $0 %d\n",addorsub,SYMBOL_STR(1),SYMBOL_INT(2));
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

void compileSingleInstruction(Inst* instruction)
{


    switch(instruction->type)
    {
        case LABEL:
            printf("%s:\n", instruction->p1->symbol.str);
            break;
        case ADD:
            printADDSUB(instruction,"add");
            break;
         case SUB:
            printADDSUB(instruction,"sub");
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