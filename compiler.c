#include <stdio.h>
#include "parser.h"

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
    printf("%p\n", root);
    if (yyparse() == 0) {
      printf("%p\n", root);
      // printf("Result = %d\n", eval(root));
    }
    return 0;
  }