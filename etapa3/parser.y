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

%token <symbol> TK_IDENTIFIER     

%token <symbol> LIT_INT           
%token <symbol> LIT_REAL          
%token <symbol> LIT_CHAR          
%token <symbol> LIT_STRING        

%token TOKEN_ERROR      

%type <ast> program
%type <ast> decl
%type <ast> dec
%type <ast> type
%type <ast> literal
%type <ast> init_v
%type <ast> paraml
%type <ast> nextParam
%type <ast> param
%type <ast> block
%type <ast> seq_command
%type <ast> seq_command_end
%type <ast> command
%type <ast> outputl
%type <ast> nextOutput
%type <ast> output
%type <ast> expr
%type <ast> argl
%type <ast> nextArg

%left '&' '|'
%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF '<' '>'
%left '+' '-'
%left '*' '/'
%left '~'

%{
    int yylex();
    int yyerror();
%}

%start program

%%

program:
    decl                                                {root=$$;astPrint(0,root);}  
    ;

decl:           
    dec decl                                            {$$=astCreate(AST_DECL,0,$1,$2,0,0);}
    |                                                   {$$=0;}
    ;

dec:
    type TK_IDENTIFIER '=' literal ';'                  {$$=astCreate(AST_DECVAR,$2,$1,$4,0,0);}
    | type TK_IDENTIFIER '[' LIT_INT ']' init_v ';'     {$$=astCreate(AST_DECVEC,$2,$1,astCreate(AST_SYMBOL,$4,0,0,0,0),$6,0);}
    | type TK_IDENTIFIER '(' paraml ')' block           {$$=astCreate(AST_DECFUNC,$2,$1,$4,$6,0);}
    ;

type:
    KW_CHAR                                             {$$=astree_create(AST_TCHAR,0,0,0,0,0);}
    | KW_INT                                            {$$=astree_create(AST_TINT,0,0,0,0,0);}
    | KW_REAL                                           {$$=astree_create(AST_TREAL,0,0,0,0,0);}
    ;

literal:
    LIT_CHAR                                            {$$=astree_create(AST_SYMBOL,$1,0,0,0,0);}
    | LIT_INT                                           {$$=astree_create(AST_SYMBOL,$1,0,0,0,0);}
    | LIT_REAL                                          {$$=astree_create(AST_SYMBOL,$1,0,0,0,0);}
    ;

init_v:
    literal init_v                                      {$$=astCreate(AST_INITV,0,$1,$2,0,0);}
    |                                                   {$$=0;}
    ;   

paraml:
    param nextParam                                     {$$=astCreate(AST_PARAML,0,$1,$2,0,0);}
    |                                                   {$$=0;}
    ;

nextParam:
    ',' param nextParam                                 {$$=astCreate(AST_NXTPARAM,0,$2,$3,0,0);}
    |                                                   {$$=0;}
    ;

param:
    type TK_IDENTIFIER                                  {$$=astCreate(AST_PARAM,$2,$1,0,0,0);}
    ;

block:
    '{' seq_command '}'                                 {$$=astCreate(AST_BLOCK,0,$2,0,0,0);}
    ;

seq_command:
    command seq_command_end                             {$$=astCreate(AST_SQCOMM,0,$1,$2,0,0);}
    |                                                   {$$=0;}
    ;

seq_command_end:
    command seq_command_end                             {$$=astCreate(AST_SQCOMMEND,0,$1,$2,0,0);}
    |                                                   {$$=0;}
    ;

command:
    ';'                                                 {$$=0;}
    | block                                             {$$=$1;}
    | TK_IDENTIFIER '=' expr ';'                        {$$=astCreate(AST_ATTRIB,$1,$3,0,0,0);}
    | TK_IDENTIFIER '[' expr ']' '=' expr ';'           {$$=astCreate(AST_VECATTR,$1,$3,$6,0,0);}
    | KW_OUTPUT outputl ';'                             {$$=astCreate(AST_OUTPUT,$2,0,0,0);}
    | KW_RETURN expr ';'                                {$$=astCreate(AST_RETURN,0,$2,0,0,0);}
    | KW_IF '(' expr ')' command                        {$$=astCreate(AST_IF,0,$3,$5,0,0);}
    | KW_IF '(' expr ')' command KW_ELSE command        {$$=astCreate(AST_IFELSE,0,$3,$5,$7,0);}
    | KW_IF '(' expr ')' KW_LOOP command                {$$astCreate(AST_IFELWHILE,0,$3,$6,0,0);}
    ;

outputl:
    output nextOutput                                   {$$=astCreate(AST_OUTPUTL,0,$1,$2,0,0);}
    ;

nextOutput:
    ',' output nextOutput                               {$$=astCreate(AST_NEXTOUT,0,$2,$3,0,0);}
    |                                                   {$$=0;}
    ;

output:
    LIT_STRING                                          {$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
    | expr                                              {$$=$1;}
    ;

expr:
    TK_IDENTIFIER                                       {$$=astCreate(AST_SYMBOL,$1,0,0,0,0);}
    | TK_IDENTIFIER '[' expr ']'                        {$$=astCreate(AST_VEC,$1,$3,0,0,0);}
    | literal                                           {$$=$1;}
    | expr '+' expr                                     {$$=astCreate(AST_ADD,0,$1,$3,0,0);}
    | expr '-' expr                                     {$$=astCreate(AST_SUB,0,$1,$3,0,0);}
    | expr '*' expr                                     {$$=astCreate(AST_MUL,0,$1,$3,0,0);}
    | expr '/' expr                                     {$$=astCreate(AST_DIV,0,$1,$3,0,0);}
    | expr '<' expr                                     {$$=astCreate(AST_LESSER,0,$1,$3,0,0);}
    | expr '>' expr                                     {$$=astCreate(AST_GREATER,0,$1,$3,0,0);}
    | expr OPERATOR_DIF expr                            {$$=astCreate(AST_DIF,0,$1,$3,0,0);}
    | expr OPERATOR_EQ expr                             {$$=astCreate(AST_EQUAL,0,$1,$3,0,0);}
    | expr OPERATOR_GE expr                             {$$=astCreate(AST_GREATOP,0,$1,$3,0,0);}
    | expr OPERATOR_LE expr                             {$$=astCreate(AST_LESSOP,0,$1,$3,0,0);}
    | expr '&' expr                                     {$$=astCreate(AST_AND,0,$1,$3,0,0);}
    | expr '|' expr                                     {$$=astCreate(AST_OR,0,$1,$3,0,0);}
    | '~' expr                                          {$$=astCreate(AST_NOT,0,$2,0,0,0);}
    | '(' expr ')'                                      {$$=astCreate(AST_PAREN,0,$2,0,0,0);}
    | TK_IDENTIFIER '(' argl ')'                        {$$=astCreate(AST_FUNC,$1,$3,0,0,0);}
    | KW_INPUT '(' type ')'                             {$$=astCreate(AST_INPUT,0,$3,0,0,0);}
    ;

argl:
    expr nextArg                                        {$$=astcreate(AST_ARGL,0,$1,$2,0,0);}
    ;

nextArg:
    ',' expr nextArg                                    {$$=astcreate(AST_NEXTARG,0,$2,$3,0,0);}
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
