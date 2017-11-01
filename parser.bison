%start prog;


%token VAR_TOKEN
%token FUNC_TOKEN
%token PRINT_FUNCTION_TOKEN
%token SCAN_FUNCTION_TOKEN
%token IF_TOKEN
%token FOR_TOKEN
%token ELSE_TOKEN
%token SEPARATOR_TOKEN
%token INT_TOKEN
%token BOOL_TOKEN
%token BINARY_TOKEN
%token INCREMENT_TOKEN 

%left ASSIGN_TOKEN
%left REL_TOKEN
%left ADD_TOKEN
%left MUL_TOKEN
%left OPENPAR_TOKEN
%left CLOSEPAR_TOKEN
%left OPENBRA_TOKEN
%left CLOSEBRA_TOKEN

// Types/values in association to grammar symbols.
%union {
    char* stringValue;
    int intValue;
    int boolValue;
}

%type <intValue> INT_TOKEN
%type <boolValue> BOOL_TOKEN
%type <stringValue> VAR_TOKEN


%code requires {
#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
}

%%

prog: FUNC_TOKEN VAR_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN

cmd_list: /* empty */
	 | cmd cmd_list

cmd: declaration SEPARATOR_TOKEN
   | increment SEPARATOR_TOKEN
   | if_only
   | if_else
   | for_one_arg
   | for_three_arg
   | print SEPARATOR_TOKEN
   | scan SEPARATOR_TOKEN

var_list: /*empty*/
        | VAR_TOKEN var_list

declaration: VAR_TOKEN ASSIGN_TOKEN expr

if_only: IF_TOKEN expr_bool OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN 

if_else: if_only ELSE_TOKEN OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN

for_one_arg: FOR_TOKEN expr_bool OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN 

for_three_arg: FOR_TOKEN declaration SEPARATOR_TOKEN expr_bool SEPARATOR_TOKEN increment OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN 

print: PRINT_FUNCTION_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN 

scan: SCAN_FUNCTION_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN 

increment: VAR_TOKEN INCREMENT_TOKEN
         | VAR_TOKEN INCREMENT_TOKEN expr

expr: expr_arit
    | expr_bool 

expr_arit: expr_arit arit_op expr_arit
	 | arit_value

arit_value: VAR_TOKEN
     	  | INT_TOKEN


expr_bool: expr_bool REL_TOKEN expr_bool
	 | expr_arit REL_TOKEN expr_arit
	 | bool_value

bool_value: VAR_TOKEN
	  | BOOL_TOKEN

arit_op: ADD_TOKEN
       | MUL_TOKEN

%%

void yyerror(const char* err) {
  printf("error: %s", err );
}

