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
%token <stringValue> BINARY_TOKEN
%token <stringValue> BINARY_REL_TOKEN
%token <stringValue> REL_TOKEN
%token <stringValue> INCREMENT_TOKEN 

%left <command> ASSIGN_TOKEN
%left <stringValue> ADD_TOKEN
%left <stringValue> MUL_TOKEN
%token PACKAGE_TOKEN
%token IMPORT_TOKEN
%token QUOTES_TOKEN 
%token COMMA_TOKEN
%precedence BEFORE_EXP
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
%type <expression> expr_var;
%type <expression> expr_bool;
%type <expression> expr_bool_final;
%type <expression> expr_arit;
%type <expression> expr_arit_final;
%type <stringValue> arit_op;
%type <stringValue> arit_comp;
%type <stringValue> bool_op;
%type <cmdList> func_list;
%type <cmdList> cmd_list;
%type <cmdList> if_else;
%type <exprList> arg_list;
%type <exprList> n_arg_list;
%type <exprList> var_list;
%type <exprList> n_var_list;
%type <command> func_declaration;
%type <command> cmd;
%type <command> declaration
%type <command> increment
%type <command> if
%type <command> for_one_arg
%type <command> for_three_arg
%type <command> print
%type <command> scan
%type <command> func_call
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
CmdList* root;
}

%%

prog: packages imports cmd_list


packages:
	| PACKAGE_TOKEN VAR_TOKEN packages

imports:
	| IMPORT_TOKEN QUOTES_TOKEN VAR_TOKEN QUOTES_TOKEN imports

func_declaration: FUNC_TOKEN VAR_TOKEN OPENPAR_TOKEN var_list CLOSEPAR_TOKEN OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = makeFunc($2->attr.variable, $4, $7);}

func_call: expr_var OPENPAR_TOKEN arg_list CLOSEPAR_TOKEN { $$ = makeFuncCall($1->attr.variable, $3);}

func_list: {$$ = EMPTY_LIST;}
	 | func_declaration func_list {$$ = root = prependCmd($2, $1);}

cmd_list: {$$ = EMPTY_LIST;};
	 | cmd cmd_list {$$ = prependCmd($2, $1);}



cmd: declaration SEPARATOR_TOKEN
   | increment SEPARATOR_TOKEN
   | if
   | for_one_arg
   | for_three_arg
   | print SEPARATOR_TOKEN
   | scan SEPARATOR_TOKEN
   | func_call SEPARATOR_TOKEN
   | func_declaration


var_list:
          | n_var_list

n_var_list: VAR_TOKEN
          | VAR_TOKEN COMMA_TOKEN n_var_list

arg_list: {$$ = EMPTY_LIST;}
	      | n_arg_list

n_arg_list: expr {$$ = makeExprList($1);}
	        | expr COMMA_TOKEN n_arg_list {$$ = prependExpr($3, $1);}

declaration: expr_var ASSIGN_TOKEN expr {$$ = makeDeclarationCmd($1, ":=", $3); /*TODO GET OPERATOR*/}

if: IF_TOKEN expr_bool OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN if_else {$$ = makeIfElseCmd($2, $4, $6); }

if_else: {$$ = NULL;}
        | ELSE_TOKEN OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = $3;}

for_one_arg: FOR_TOKEN expr_bool OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = makeFor(NULL,$2,NULL,$4);}

for_three_arg: FOR_TOKEN declaration SEPARATOR_TOKEN expr_bool SEPARATOR_TOKEN increment OPENBRA_TOKEN cmd_list CLOSEBRA_TOKEN {$$ = makeFor($2,$4,$6,$8);}

print: PRINT_FUNCTION_TOKEN OPENPAR_TOKEN arg_list CLOSEPAR_TOKEN  { $$ = makeFuncCall("fmt.print", $3);}

scan: SCAN_FUNCTION_TOKEN OPENPAR_TOKEN arg_list CLOSEPAR_TOKEN { $$ = makeFuncCall("fmt.scan", $3);}

increment: expr_var INCREMENT_TOKEN {$$ = makeIncrementCmd($1, $2, NULL);}
         | expr_var INCREMENT_TOKEN expr {$$ = makeIncrementCmd($1, $2, $3);}

expr: expr_arit {$$ = $1;}
    | expr_bool {$$ = $1;}

expr_arit: expr_arit arit_op expr_arit_final {$$ = ast_operation($2, $1, $3);}
    | expr_arit_final 

expr_arit_final: expr_var
    | INT_TOKEN { $$ = ast_integer(yylval.intValue);}

expr_bool: expr_bool bool_op expr_bool_final {$$ = ast_operation($2, $1, $3);}
    | expr_arit arit_comp expr_arit {$$ = ast_operation($2, $1, $3);}
    | expr_bool_final

expr_bool_final: BOOL_TOKEN { $$ = ast_bool(yylval.boolValue);}

bool_op: REL_TOKEN
    |   BINARY_REL_TOKEN

arit_comp: BINARY_REL_TOKEN
    |   REL_TOKEN

arit_op: ADD_TOKEN
       | MUL_TOKEN 
expr_var: VAR_TOKEN { $$ = ast_variable(yylval.stringValue);}

%%

void yyerror(const char* err) {
  printf("error: %s\n", err );
}

