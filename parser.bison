%start prog;


%token <expression> VAR_TOKEN
%token <command> FUNC_TOKEN
%token PRINT_FUNCTION_TOKEN
%token SCAN_FUNCTION_TOKEN
%token IF_TOKEN
%token FOR_TOKEN
%token ELSE_TOKEN
%token SEPARATOR_TOKEN
%token <expression> INT_TOKEN
%token <expression> BOOL_TOKEN
%token BINARY_TOKEN
%token <stringValue> INCREMENT_TOKEN 

%left <command> ASSIGN_TOKEN
%left <stringValue> REL_TOKEN
%left <stringValue> ADD_TOKEN
%left <stringValue> MUL_TOKEN
%left OPENPAR_TOKEN
%left CLOSEPAR_TOKEN
%left OPENBRA_TOKEN
%left CLOSEBRA_TOKEN

// Types/values in association to grammar symbols.
%union {
    char* stringValue;
    int intValue;
    int boolValue;
    Expr* expression;
    Cmd* command;
    ExprList* exprList;
    CmdList* cmdList;
}
/*
%type <intValue> INT_TOKEN
%type <boolValue> BOOL_TOKEN
%type <stringValue> VAR_TOKEN
*/
%type <expression> expr;
%type <expression> bool_value;
%type <expression> arit_value;
%type <expression> expr_bool;
%type <expression> expr_arit;
%type <stringValue> arit_op;
%type <cmdList> cmd_list;
%type <cmdList> if_else;
%type <exprList> var_list;
%type <command> cmd;
%type <command> declaration
%type <command> increment
%type <command> if
%type <command> for_one_arg
%type <command> for_three_arg
%type <command> print
%type <command> scan
/*
%type <Expr*> VAR_TOKEN
%type <Expr*> BOOL_TOKEN
%type <Expr*> INT_TOKEN
%type <Expr*> bool_value*/

%code requires {
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "cmd.h"

extern int yylex();
extern char* yytext;
extern FILE* yyin;
extern void yyerror(const char* msg);
}

%%

prog: FUNC_TOKEN VAR_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN

cmd_list: {$$ = EMPTY_LIST;};
	 | cmd cmd_list {$$ = appendCmd($2, $1);}

cmd: declaration SEPARATOR_TOKEN
   | increment SEPARATOR_TOKEN
   | if
   | for_one_arg
   | for_three_arg
   | print SEPARATOR_TOKEN
   | scan SEPARATOR_TOKEN

var_list: {$$ = EMPTY_LIST;}
        | VAR_TOKEN var_list {$$ = appendExpr($2, $1);}

declaration: VAR_TOKEN ASSIGN_TOKEN expr { $$ = makeDeclarationCmd($1, $3);}

if: IF_TOKEN expr_bool OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN if_else {$$ = makeIfElseCmd($2, $4, $6); }

if_else: {$$ = NULL;}
        | ELSE_TOKEN OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = $3;}

for_one_arg: FOR_TOKEN expr_bool OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = makeFor(NULL,$2,NULL,$4);}

for_three_arg: FOR_TOKEN declaration SEPARATOR_TOKEN expr_bool SEPARATOR_TOKEN increment OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = makeFor($2,$4,$6,$8);}

print: PRINT_FUNCTION_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN  { $$ = makeFuncCall("fmt.print", $3);}

scan: SCAN_FUNCTION_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN { $$ = makeFuncCall("fmt.scan", $3);}

increment: VAR_TOKEN INCREMENT_TOKEN {$$ = makeIncrementCmd($1, $2, NULL);}
         | VAR_TOKEN INCREMENT_TOKEN expr {$$ = makeIncrementCmd($1, $2, $3);}

expr: expr_arit {$$ = $1;}
    | expr_bool {$$ = $1;}

expr_arit: expr_arit arit_op expr_arit {$$ = ast_operation($2, $1, $3);}
	 | arit_value {$$ = $1;}

arit_value: VAR_TOKEN  { $$ = ast_variable(yylval.stringValue);}
     	  | INT_TOKEN {$$ = ast_bool(yylval.boolValue);}


expr_bool: expr_bool REL_TOKEN expr_bool {$$ = ast_operation($2, $1, $3);}
	 | expr_arit REL_TOKEN expr_arit {$$ = ast_operation($2, $1, $3);}
	 | bool_value {$$ = $1;}

bool_value: VAR_TOKEN { $$ = ast_variable(yylval.stringValue);}
	  | BOOL_TOKEN {$$ = ast_bool(yylval.boolValue);}

arit_op: ADD_TOKEN
       | MUL_TOKEN

%%

void yyerror(const char* err) {
  printf("error: %s", err );
}

