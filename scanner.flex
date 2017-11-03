%{
// HEADERS
#include <stdlib.h>
#include <string.h>
#include "parser.h"
%}

%option noyywrap
%%

\/\/.*\n {
            //one line comment
         }

:=|=  {printf("ASSIGN_TOKEN\n");return ASSIGN_TOKEN;}

"package" {printf("PACKAGE_TOKEN\n");return PACKAGE_TOKEN;}

"import" {printf("IMPORT_TOKEN\n");return IMPORT_TOKEN;}

\" {printf("QUOTES_TOKEN\n");return QUOTES_TOKEN;}

"," {printf("COMMA_TOKEN\n");return COMMA_TOKEN;}

\|\||&& {
	yylval.stringValue = strdup(yytext);
    printf("BINARY_TOKEN\n");
	return BINARY_TOKEN;
}

==|!=|<|<=|>|>= { 
	yylval.stringValue = strdup(yytext);
    printf("REL_TOKEN\n");
	return REL_TOKEN;
}

\+=|-=|\+\+|-- {
    yylval.stringValue = strdup(yytext);
    printf("INCREMENT_TOKEN\n");
    return INCREMENT_TOKEN;
}

\+|-|\\|\^ {
	yylval.stringValue = strdup(yytext);
    printf("ADD_TOKEN\n");
	return ADD_TOKEN;
}

\*|\/|%|<<|>>|&|&^ {
	yylval.stringValue = strdup(yytext);
    printf("MUL_TOKEN\n");
	return MUL_TOKEN;
}

"fmt\.Print" {printf("PRINT_FUNCTION_TOKEN\n");return PRINT_FUNCTION_TOKEN;}

"fmt\.Scan" {printf("SCAN_FUNCTION_TOKEN\n");return SCAN_FUNCTION_TOKEN;}


"if" {printf("IF_TOKEN\n");return IF_TOKEN;}

"else" {printf("ELSE_TOKEN\n");return ELSE_TOKEN;}

"for" {printf("     FOR_TOKEN\n");return FOR_TOKEN;}

"(" {printf("( spotted \n");return OPENPAR_TOKEN; }
")" {printf(") spotted \n");return CLOSEPAR_TOKEN; }

"{" {printf("OPENBRA_TOKEN\n");return OPENBRA_TOKEN;}
"}" {printf("CLOSEBRA_TOKEN\n");return CLOSEBRA_TOKEN;}

true|false {
    if(strcmp(yytext,"true")==0){
        yylval.boolValue = 1;
    }
    else{
        yylval.boolValue = 0;
    } 
    printf("BOOL_TOKEN\n");
    return BOOL_TOKEN;
}

"func" { printf("FUNC FOUND\n"); return FUNC_TOKEN; }

";" { printf("SEPARATOR_TOKEN\n");return SEPARATOR_TOKEN; }

[0-9]+ {
    yylval.intValue = atoi(yytext);
    printf("INT_TOKEN\n");
    return INT_TOKEN;
}

[a-zA-Z_]+[a-zA-Z_0-9]* {
    yylval.stringValue = strdup(yytext);
    printf("FOUND VAR: %s\n",yylval.stringValue);
    return VAR_TOKEN;
}

[ \t\n\r] {}

%%
