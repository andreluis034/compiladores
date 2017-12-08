%{
// HEADERS
#include <stdlib.h>
#include <string.h>
#include "parser.h"

#define sumChars yycharCount += strlen(yytext)
#define resetChars yycharCount = 0
#define sumLines linesCount ++

int linesCount = 0;
int yycharCount = 0;
%}


%option noyywrap
%%

\/\/.*\n {
            //one line comment
         }

:=|=  { 
         //printf("ASSIGN_TOKEN\n");
        yylval.stringValue = strdup(yytext);
        sumChars;
        return ASSIGN_TOKEN;
}

"package" {
             //printf("PACKAGE_TOKEN\n");
             sumChars;
             return PACKAGE_TOKEN;
          }

"import" {
            //printf("IMPORT_TOKEN\n");
            sumChars;
            return IMPORT_TOKEN;
         }

\"  {
        //printf("QUOTES_TOKEN\n");
        sumChars;
        return QUOTES_TOKEN;
    }

"," {
        //printf("COMMA_TOKEN\n");
        sumChars;
        return COMMA_TOKEN;
    }

\|\||&& {
        yylval.stringValue = strdup(yytext);
        //printf("BINARY_TOKEN\n");
        sumChars;
        return BINARY_TOKEN;
}

==|!= { 
	yylval.stringValue = strdup(yytext);
    //printf("BINARY_REL_TOKEN\n");
    sumChars;
	return BINARY_REL_TOKEN;
}
\<|\<\=|\>|\>\= { 
	yylval.stringValue = strdup(yytext);
    //printf("REL_TOKEN\n");
	return REL_TOKEN;
}

\+=|-=|\+\+|-- {
        yylval.stringValue = strdup(yytext);
        //printf("INCREMENT_TOKEN\n");
        sumChars;
        return INCREMENT_TOKEN;
}

\+|-|\\|\^ {
        yylval.stringValue = strdup(yytext);
        //printf("ADD_TOKEN\n");
        sumChars;
        return ADD_TOKEN;
}

\*|\/|%|<<|>>|&|&^ {
        yylval.stringValue = strdup(yytext);
        //printf("MUL_TOKEN\n");
        sumChars;
        return MUL_TOKEN;
}

"fmt\.Print" {   
                //printf("PRINT_FUNCTION_TOKEN\n");
                sumChars;
                return PRINT_FUNCTION_TOKEN;
             }

"fmt\.Scan" {
                //printf("SCAN_FUNCTION_TOKEN\n");
                sumChars;
                return SCAN_FUNCTION_TOKEN;
            }


"if" {
        //printf("IF_TOKEN\n");
        sumChars;
        return IF_TOKEN;
     }

"else" {
        //printf("ELSE_TOKEN\n");
        sumChars;
        return ELSE_TOKEN;
       }

"for" {
        //printf("FOR_TOKEN\n");
        sumChars;
        return FOR_TOKEN;
      }

"(" {
        //printf("( spotted \n");
        sumChars;
        return OPENPAR_TOKEN;
    }

")" {
        //printf(") spotted \n");
        sumChars;
        return CLOSEPAR_TOKEN; 
    }

"{" {
        //printf("OPENBRA_TOKEN\n");
        sumChars;
        return OPENBRA_TOKEN;
    }

"}" {
        //printf("CLOSEBRA_TOKEN\n");
        sumChars;
        return CLOSEBRA_TOKEN;}

true|false {
    if(strcmp(yytext,"true")==0){
        yylval.boolValue = 1;
    }
    else{
        yylval.boolValue = 0;
    } 
    //printf("BOOL_TOKEN\n");
    sumChars;
    return BOOL_TOKEN;
}

"func" { 
        //printf("FUNC FOUND\n");
        sumChars;
        return FUNC_TOKEN;
       }

"return" {
        sumChars;
        return RETURN_TOKEN;
       }       

";" { 
        //printf("SEPARATOR_TOKEN\n");
        sumChars;
        return SEPARATOR_TOKEN;
    }

[0-9]+ {
    yylval.intValue = atoi(yytext);
    //printf("INT_TOKEN\n");
    sumChars;
    return INT_TOKEN;
}

[a-zA-Z_]+[a-zA-Z_0-9]* {
    yylval.stringValue = strdup(yytext);
    //printf("FOUND VAR: %s\n",yylval.stringValue);
    sumChars;
    return VAR_TOKEN;
}
[\n\r] {
        resetChars;
        sumLines;
     }

[ \t] {
            sumChars;
        }

%%
