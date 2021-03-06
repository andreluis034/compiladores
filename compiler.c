#include <stdio.h>
#include "parser.h"
#include "mips.h"
#include "threeAddressCode.h"
int main(int argc, char** argv) {
    --argc; ++argv;
    if (argc != 0) {
      yyin = fopen(*argv, "r");
      if (!yyin) {
        printf("'%s': could not open file\n", *argv);
        return 1;
      }
    } //  yyin = stdin
    root == NULL;
    initializeRegisters();
    if (yyparse() == 0) {
      
      compileToMips(compileCmdList(root),root);
      //compileCmdList(root);
      //printCmdList(root,0,1);
      // printf("Result = %d\n", eval(root));
    }
    return 0;
  }