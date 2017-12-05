#include <stdio.h>
#include "mips.h"

void compileSingleInstruction(Inst* instruction)
{
    switch(instruction->type)
    {
        case LABEL:
            printf("%s:\n", instruction->p1->symbol.str);
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