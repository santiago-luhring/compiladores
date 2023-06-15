%{
int yylex();
iny yyerror();
%}

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

%token TK_IDENTIFIER     

%token LIT_INT           
%token LIT_REAL          
%token LIT_CHAR          
%token LIT_STRING        

%token TOKEN_ERROR      

%left OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_DIF
%left '<' '>'
%left '+' '-'
%left '*' '/'
%left '&' '|' '~'

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
    | literal init_v
    ;   

paraml:
    | param nextParam
    ;

nextParam:
    | ',' param nextParam
    ;

param:
    type TK_IDENTIFIER
    ;

block:
    '{' seq_command '}'
    ;

seq_command:
    command seq_command_end
    ;

seq_command_end:
    | command seq_command_end
    ;

command:
    ';'
    | block
    | TK_IDENTIFIER '=' expr ';'
    | TK_IDENTIFIER '[' expr ']' '=' expr ';'
    | KW_OUTPUT outputl ';'
    | KW_RETURN expr ';'
    | KW_IF '(' expr ')' ifCases ';'
    ;

outputl:
    output nextOutput
    ;

nextOutput:
    | ',' output nextOutput
    ;

output:
    LIT_STRING
    | expr
    ;

ifCases:
    command
    | command KW_ELSE command
    | KW_LOOP command
    ;

expr:
    TK_IDENTIFIER
    | TK_IDENTIFIER '[' expr ']'
    | literal
    | expr op expr
    | '(' expr ')'
    | TK_IDENTIFIER '(' argl ')'
    | KW_INPUT '(' type ')'
    ;

op:
    '+'
    | '-'
    | '*'
    | '/'
    | '<'
    | '>'
    | OPERATOR_DIF
    | OPERATOR_EQ
    | OPERATOR_GE
    | OPERATOR_LE
    | '&'
    | '|'
    | '~'
    ;

argl:
    expr nextArg
    ;

nextArg:
    | ',' expr nextArg
    ;

%%

int yyerror() {

    fprintf(stdout, "\nER: Syntax error at line %d", getLineNumber());
    fprintf(stdout, "\nCompilation exit. CODE 3.\n");

    exit(3);
}
