#include "hash.h"
#include "treeAST.h"
#include <stdio.h>
#include <stdlib.h>

AST *astCreate(int type, HASH_NODE *symbol, AST* c0, AST* c1, AST* c2, AST* c3, int linenumber)
{
    AST*newnode;
    newnode = (AST*) calloc(1,sizeof(AST));
    newnode->type = type;
    newnode->symbol = symbol;
    newnode->lineNumber = linenumber;
    newnode->child[0] = c0;
    newnode->child[1] = c1;
    newnode->child[2] = c2;
    newnode->child[3] = c3;
    return newnode;
}

void astPrint(int level, AST *node)
{   
    if(node == NULL)
        return;
    
    int i = 0;

    for(i = 0; i < level; i++)
        fprintf(stderr, " ");
    fprintf(stderr, "AST(");
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
    case AST_ENDBLOCK  :  fprintf(stderr,"AST_COMMAND"); break;
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
    case AST_TBOOL     :  fprintf(stderr,"AST_TBOOL"); break;
    default: fprintf(stderr, "AST_UNKNOWN"); break;
    }
    if(node->symbol != NULL)
        fprintf(stderr, ", %s )\n", node->symbol->text);
    else
        fprintf(stderr, ")\n");

    for(i=0; i < MAX_CHILD; i++)
        astPrint(level+1, node->child[i]);

    fprintf(stderr, "\n");
}

void decompileAST(AST *node, FILE *file){
    if(node == NULL)
        return;
    switch (node->type)
    {
        case AST_DECL      :
            decompileAST(node->child[0],file);
            decompileAST(node->child[1],file); 
            break;
        case AST_DECVAR    :
            decompileAST(node->child[0],file);
            fprintf(file,"%s = ",node->symbol->text);
            decompileAST(node->child[1],file);
            fprintf(file,";\n");
            break;
        case AST_DECVEC    :
            decompileAST(node->child[0],file);
            fprintf(file,"%s[",node->symbol->text);
            decompileAST(node->child[1],file);
            fprintf(file,"]");
            decompileAST(node->child[2],file);
            fprintf(file,";\n");
            break;
        case AST_DECFUNC   : 
            decompileAST(node->child[0], file);
            fprintf(file, " %s(", node->symbol->text);
            decompileAST(node->child[1], file);
            fprintf(file, ")");      
            decompileAST(node->child[2], file); 
            break;
        case AST_TCHAR     :    
            fprintf(file,"char "); break;
        case AST_TINT      :  
            fprintf(file,"int "); break;
        case AST_TREAL     :  
            fprintf(file,"real "); break;
        case AST_SYMBOL    :  
            fprintf(file, " %s ", node->symbol->text);
            break;
        case AST_INITV     :  
            decompileAST(node->child[0], file);
            fprintf(file, " ");
            decompileAST(node->child[1], file);
            break;
        case AST_PARAML    :  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file);
            break;
        case AST_NXTPRM    :  
            fprintf(file, ", ");
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file);
            break;
        case AST_PARAM     :  
            decompileAST(node->child[0], file);
            fprintf(file, " ");
            fprintf(file, "%s", node->symbol->text); 
            break;
        case AST_BLOCK     :  
            fprintf(file, "{\n");
            decompileAST(node->child[0], file);
            fprintf(file, "}\n");
            break;
        case AST_SQCOMM    :  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file);
            break;
        case AST_SQCOMMEND :  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file);
            break;
        case AST_ENDBLOCK  :  
            fprintf(file,";\n"); 
            break;
        case AST_ATTRIB    :  
            fprintf(file, "%s =", node->symbol->text);
            decompileAST(node->child[0], file);
            fprintf(file,";\n"); 
            break;
        case AST_VECATTR   :  
            fprintf(file,"%s[",node->symbol->text);
            decompileAST(node->child[0],file);
            fprintf(file,"] = ");
            decompileAST(node->child[1],file);
            fprintf(file,";\n"); 
            break;
        case AST_OUTPUT    :  
            fprintf(file,"output ");
            decompileAST(node->child[0],file); 
            fprintf(file,";\n"); 
            break;
        case AST_RETURN    :  
            fprintf(file,"return ");
            decompileAST(node->child[0],file); 
            fprintf(file,";\n");  
            break;
        case AST_IF        :  
            fprintf(file, "if(");
            decompileAST(node->child[0], file);
            fprintf(file, ")\n");
            decompileAST(node->child[1], file);
            break;
        case AST_IFELSE    :  
            fprintf(file, "if(");
            decompileAST(node->child[0], file);
            fprintf(file, ")\n");
            decompileAST(node->child[1], file);
            fprintf(file, "else\n");
            decompileAST(node->child[2], file);
            break;
        case AST_IFELWHILE :  
            fprintf(file, "if(");
            decompileAST(node->child[0], file);
            fprintf(file, ")\n loop\n");
            decompileAST(node->child[1], file);
            break;
        case AST_OUTPUTL   :  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file); 
            break;
        case AST_NEXTOUT   :
            fprintf(file, ", ");  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file); 
            break;
        case AST_VEC       :  
            fprintf(file,"%s[",node->symbol->text);
            decompileAST(node->child[0],file);
            fprintf(file,"]");
            break;
        case AST_ADD       :  
            decompileAST(node->child[0], file);
            fprintf(file, " + ");
            decompileAST(node->child[1], file);
            break;
        case AST_SUB       :  
            decompileAST(node->child[0], file);
            fprintf(file, " - ");
            decompileAST(node->child[1], file);
            break;
        case AST_MUL       :  
            decompileAST(node->child[0], file);
            fprintf(file, " * ");
            decompileAST(node->child[1], file);
            break;
        case AST_DIV       :  
            decompileAST(node->child[0], file);
            fprintf(file, " / ");
            decompileAST(node->child[1], file);
            break;
        case AST_LESSER    :  
            decompileAST(node->child[0], file);
            fprintf(file, " < ");
            decompileAST(node->child[1], file);
            break;
        case AST_GREATER   :  
            decompileAST(node->child[0], file);
            fprintf(file, " > ");
            decompileAST(node->child[1], file);
            break;
        case AST_DIF       :  
            decompileAST(node->child[0], file);
            fprintf(file, " != ");
            decompileAST(node->child[1], file);
            break;
        case AST_EQUAL     :  
            decompileAST(node->child[0], file);
            fprintf(file, " == ");
            decompileAST(node->child[1], file);
            break;
        case AST_GREATOP   :  
            decompileAST(node->child[0], file);
            fprintf(file, " >= ");
            decompileAST(node->child[1], file);
            break;
        case AST_LESSOP    :  
            decompileAST(node->child[0], file);
            fprintf(file, " <= ");
            decompileAST(node->child[1], file);
            break;
        case AST_AND       :  
            decompileAST(node->child[0], file);
            fprintf(file, " & ");
            decompileAST(node->child[1], file);
            break;
        case AST_OR        :  
            decompileAST(node->child[0], file);
            fprintf(file, " | ");
            decompileAST(node->child[1], file);
            break;
        case AST_NOT       :  
            fprintf(file,"~");
            decompileAST(node->child[0], file); 
            break;
        case AST_PAREN     :  
            fprintf(file, "(");
            decompileAST(node->child[0], file);
            fprintf(file, ")"); 
            break;
        case AST_FUNC      :  
            fprintf(file,"%s(",node->symbol->text);
            decompileAST(node->child[0],file);
            fprintf(file,")"); 
            break;
        case AST_INPUT     :  
            fprintf(file, " input (");
            decompileAST(node->child[0], file);
            fprintf(file, ")");
            break;
        case AST_ARGL      :  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file);
            break;
        case AST_NEXTARG   :  
            fprintf(file, ", ");  
            decompileAST(node->child[0], file);
            decompileAST(node->child[1], file);
            break;
        case AST_TBOOL     :    
            fprintf(file,"bool "); break; 

    }

}