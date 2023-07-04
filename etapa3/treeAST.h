#include "hash.h"
#ifndef AST_HEADER
#define AST_HEADER


#define MAX_CHILD 4

#define AST_DECL        1
#define AST_DECVAR      2
#define AST_DECVEC      3
#define AST_DECFUNC     4
#define AST_TCHAR       5
#define AST_TINT        6
#define AST_TREAL       7
#define AST_SYMBOL      8
#define AST_INITV       9
#define AST_PARAML      10
#define AST_NXTPRM      11
#define AST_PARAM       12
#define AST_BLOCK       13
#define AST_SQCOMM      14
#define AST_SQCOMMEND   15
#define AST_ENDBLOCK    16
#define AST_ATTRIB      17
#define AST_VECATTR     18
#define AST_OUTPUT      19
#define AST_RETURN      20
#define AST_IF          21
#define AST_IFELSE      22
#define AST_IFELWHILE   23
#define AST_OUTPUTL     24
#define AST_NEXTOUT     25
#define AST_VEC         26
#define AST_ADD         27
#define AST_SUB         28
#define AST_MUL         29
#define AST_DIV         30
#define AST_LESSER      31
#define AST_GREATER     32
#define AST_DIF         33
#define AST_EQUAL       34
#define AST_GREATOP     35
#define AST_LESSOP      36
#define AST_AND         37
#define AST_OR          38
#define AST_NOT         39
#define AST_PAREN       40
#define AST_FUNC        41
#define AST_INPUT       42
#define AST_ARGL        43
#define AST_NEXTARG     44

typedef struct astnode
{
    int type;
    struct astnode *child[MAX_CHILD];
    HASH_NODE *symbol;
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST* c0, AST* c1, AST* c2, AST* c3);
void astPrint(int level, AST *node);
void decompileAST(AST *node, FILE *file);

#endif