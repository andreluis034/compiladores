#pragma once
#include "threeAddressCode.h"

void compileToMips(InstList* instructionList,CmdList* cmdlist) ;
void initializeRegisters() ;
void getVariables(CmdList* cmdlist) ;