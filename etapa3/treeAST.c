#include "treeAST.h"
#include <stdio.h>
#include <stdlib.h>

AST *astCreate(int type, HASH_NODE *symbol, AST* c0, AST* c1, AST* c2, AST* c3)
{
    AST*newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->child[0] = c0;
    newnode->child[1] = c1;
    newnode->child[2] = c2;
    newnode->child[3] = c3;
    return newnode;
}

void astPrint(int level, AST *node)
{   
    if(!node)
        return;
    
    int i = 0;

    for(i = 0; i < level; i++)
        fprintf(stderr, "  ");
    
    switch (node->type)
    {
    case AST_SYMBOL: fprintf(stderr,"AST_SYMBOL"); break;
    case AST_SUM: fprintf(stderr,"AST_SUM"); break;
    case AST_SUB: fprintf(stderr,"AST_SUB"); break;
    default: fprintf(stderr, "AST_UNKNOWN"); break;
    }
    if(node->symbol !=0)
        fprintf(stderr, "%s", node->symbol->text);
    else
        fprintf(stderr, "oh!");
    

    for(i=0; i < MAX_CHILD; i++)
        astPrint(level+1, node->child[i]);

    fprintf(stderr, "\n");
}