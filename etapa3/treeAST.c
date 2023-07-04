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
    case AST_DECL      :  fprintf(stderr,"AST_DECL"); break;
    case AST_DECVAR    :  fprintf(stderr,"AST_DECVAR"); break;
    case AST_DECVEC    :  fprintf(stderr,"AST_DECVEC"); break;
    case AST_DECFUNC   :  fprintf(stderr,"AST_DECFUNC"); break;
    case AST_TCHAR     :  fprintf(stderr,"AST_TCHAR"); break;
    case AST_TINT      :  fprintf(stderr,"AST_TINT"); break;
    case AST_TREAL     :  fprintf(stderr,"AST_TREAL"); break;
    case AST_SYMBOL    :  fprintf(stderr,"AST_SYMBOL"); break;
    case AST_INITV     :  fprintf(stderr,"AST_INITV"); break;
    case AST_PARAML    :  fprintf(stderr,"AST_PARAML"); break;
    case AST_NXTPRM    :  fprintf(stderr,"AST_NXTPRM"); break;
    case AST_PARAM     :  fprintf(stderr,"AST_PARAM"); break;
    case AST_BLOCK     :  fprintf(stderr,"AST_BLOCK"); break;
    case AST_SQCOMM    :  fprintf(stderr,"AST_SQCOMM"); break;
    case AST_SQCOMMEND :  fprintf(stderr,"AST_SQCOMMEND"); break;
    case AST_COMMAND   :  fprintf(stderr,"AST_COMMAND"); break;
    case AST_ATTRIB    :  fprintf(stderr,"AST_ATTRIB"); break;
    case AST_VECATTR   :  fprintf(stderr,"AST_VECATTR"); break;
    case AST_OUTPUT    :  fprintf(stderr,"AST_OUTPUT"); break;
    case AST_RETURN    :  fprintf(stderr,"AST_RETURN"); break;
    case AST_IF        :  fprintf(stderr,"AST_IF"); break;
    case AST_IFELSE    :  fprintf(stderr,"AST_IFELSE"); break;
    case AST_IFELWHILE :  fprintf(stderr,"AST_IFELWHILE"); break;
    case AST_OUTPUTL   :  fprintf(stderr,"AST_OUTPUTL"); break;
    case AST_NEXTOUT   :  fprintf(stderr,"AST_NEXTOUT"); break;
    case AST_VEC       :  fprintf(stderr,"AST_VEC"); break;
    case AST_ADD       :  fprintf(stderr,"AST_ADD"); break;
    case AST_SUB       :  fprintf(stderr,"AST_SUB"); break;
    case AST_MUL       :  fprintf(stderr,"AST_MUL"); break;
    case AST_DIV       :  fprintf(stderr,"AST_DIV"); break;
    case AST_LESSER    :  fprintf(stderr,"AST_LESSER"); break;
    case AST_GREATER   :  fprintf(stderr,"AST_GREATER"); break;
    case AST_DIF       :  fprintf(stderr,"AST_DIF"); break;
    case AST_EQUAL     :  fprintf(stderr,"AST_EQUAL"); break;
    case AST_GREATOP   :  fprintf(stderr,"AST_GREATOP"); break;
    case AST_LESSOP    :  fprintf(stderr,"AST_LESSOP"); break;
    case AST_AND       :  fprintf(stderr,"AST_AND"); break;
    case AST_OR        :  fprintf(stderr,"AST_OR"); break;
    case AST_NOT       :  fprintf(stderr,"AST_NOT"); break;
    case AST_PAREN     :  fprintf(stderr,"AST_PAREN"); break;
    case AST_FUNC      :  fprintf(stderr,"AST_FUNC"); break;
    case AST_INPUT     :  fprintf(stderr,"AST_INPUT"); break;
    case AST_ARGL      :  fprintf(stderr,"AST_ARGL"); break;
    case AST_NEXTARG   :  fprintf(stderr,"AST_NEXTARG"); break;
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