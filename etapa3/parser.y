%{
    #include "hash.h"
    #include "treeAST.h"

    int getLineNumber();

    AST* ast = NULL;
%}

%union{
    AST *ast;
    HASH_NODE *symbol;
}

%token KW_CHAR           
%token KW_INT            
%token KW_REAL           
%token KW_BOOL           

%token KW_IF             
%token KW_THEN           
%token KW_ELSE 

%token KW_LOOP

%token KW_INPUT          
%token KW_OUTPUT         
%token KW_RETURN         

%token OPERATOR_LE       
%token OPERATOR_GE       
%token OPERATOR_EQ       
%token OPERATOR_DIF      

%token<symbol> TK_IDENTIFIER     

%token<symbol> LIT_INT           
%token<symbol> LIT_REAL          
%token<symbol> LIT_CHAR          
%token<symbol> LIT_STRING        

%token TOKEN_ERROR      

%type<ast>program
%type<ast>decl
%type<ast>dec
%type<ast>type
%type<ast>literal
%type<ast>init_v
%type<ast>paraml
%type<ast>nextParam
%type<ast>param
%type<ast>block
%type<ast>seq_command
%type<ast>seq_command_end
%type<ast>command
%type<ast>outputl
%type<ast>nextOutput
%type<ast>output
%type<ast> expr
%type<ast>argl
%type<ast>nextArg

%left '&' '|'
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>'
%left '+' '-'
%left '*' '/'
%left '~'

%{
    int yylex();
    int yyerror();
%}

%%

program:
    decl
    ;

decl:
    dec decl
    |
    ;

dec:
    type TK_IDENTIFIER '=' literal ';'
    | type TK_IDENTIFIER '[' LIT_INT ']' init_v ';'
    | type TK_IDENTIFIER '(' paraml ')' block 
    ;

type:
    KW_CHAR
    | KW_INT
    | KW_REAL
    ;

literal:
    LIT_CHAR
    | LIT_INT
    | LIT_REAL
    ;

init_v:
    literal init_v
    |
    ;   

paraml:
    param nextParam
    |
    ;

nextParam:
    ',' param nextParam
    |
    ;

param:
    type TK_IDENTIFIER
    ;

block:
    '{' seq_command '}'
    ;

seq_command:
    command seq_command_end
    |
    ;

seq_command_end:
    command seq_command_end
    |
    ;

command:
    ';'
    | block
    | TK_IDENTIFIER '=' expr ';'
    | TK_IDENTIFIER '[' expr ']' '=' expr ';'
    | KW_OUTPUT outputl ';'
    | KW_RETURN expr ';'
    | KW_IF '(' expr ')' command
    | KW_IF '(' expr ')' command KW_ELSE command
    | KW_IF '(' expr ')' KW_LOOP command
    ;

outputl:
    output nextOutput
    ;

nextOutput:
    ',' output nextOutput
    |
    ;

output:
    LIT_STRING
    | expr
    ;

expr:
    TK_IDENTIFIER                   {$$ = astCreate(AST_SYMBOL,$1,0,0,0,0);}
    | TK_IDENTIFIER '[' expr ']'
    | literal
    | expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr '<' expr
    | expr '>' expr
    | expr OPERATOR_DIF expr
    | expr OPERATOR_EQ expr
    | expr OPERATOR_GE expr
    | expr OPERATOR_LE expr
    | expr '&' expr
    | expr '|' expr
    | expr '~' expr
    | '(' expr ')'                  {$$ = $2;}
    | TK_IDENTIFIER '(' argl ')'
    | KW_INPUT '(' type ')'
    ;

argl:
    expr nextArg
    ;

nextArg:
    ',' expr nextArg
    |
    ;

%%

#include <stdio.h>
#include <stdlib.h>

int yyerror() {

    fprintf(stdout, "\nER: Syntax error at line %d", getLineNumber());
    fprintf(stdout, "\nCompilation exit. CODE 3.\n");

    exit(3);
}
