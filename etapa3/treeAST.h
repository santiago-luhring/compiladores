#include "hash.h"
#ifndef AST_HEADER
#define AST_HEADER


#define MAX_CHILD 4
#define AST_SYMBOL 1
#define AST_SUM 2
#define AST_SUB 3
typedef struct astnode
{
    int type;
    struct astnode *child[MAX_CHILD];
    HASH_NODE *symbol;
} AST;

AST *astCreate(int type, HASH_NODE *symbol, AST* c0, AST* c1, AST* c2, AST* c3);
void astPrint(int level, AST *node);

#endif