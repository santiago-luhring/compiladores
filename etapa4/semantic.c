#include "semantic.h"

int SemanticErrors = 0;

void check_and_set_declaration(AST *node)
{
    if(node == 0)
        return;

    switch (node->type)
    {
    case AST_DECVAR:
        set_declaration(node, AST_DECVAR);
        break;
    case AST_DECFUNC:
        set_declaration(node, AST_DECFUNC);
        break;
    case AST_DECVEC:
        set_declaration(node, AST_DECVEC);
        break;
    case AST_PARAM:
        set_declaration(node, AST_PARAM);
        break;
    }
    for (int i = 0; i < MAX_CHILD; i++)
        check_and_set_declaration(node->child[i]);
}

void set_declaration(AST *node, int symbol)
{
    if(node->symbol->type != SYMBOL_IDENTIFIER)
        {
            fprintf(stderr,"Semantic ERROR in line %d: redeclaration of %s !\n",node->lineNumber, node->symbol->text);
        }
        else
        {
            node->symbol->type = symbol;
            if(node->child[0]->type == AST_TCHAR) node->symbol->datatype = DATATYPE_CHAR;
            else if(node->child[0]->type == AST_TINT) node->symbol->datatype = DATATYPE_INT;
            else if(node->child[0]->type == AST_TREAL) node->symbol->datatype = DATATYPE_REAL;
        }
        if(symbol == AST_DECVAR)
        {
            if(!isDatatypeCompatible(node->symbol->datatype, node->child[1]->symbol->datatype))
            {
                fprintf(stderr, "Semantic ERROR in line %d: Variable declaration with mixed dataypes\n", node->lineNumber);
                SemanticErrors++;
            }
        }
        if (symbol == AST_DECVEC)
        {
           if(!check_vec_elements(node->child[2], node->symbol->datatype))
           {
			    fprintf(stderr, "Semantic ERROR on line %d: Vector declaration with elements of mixed datatypes\n", node->lineNumber);
			    SemanticErrors++;
		   } 
        }
        
        
}

void check_undeclared()
{
    int undeclared = hash_check_undeclared();
    SemanticErrors += undeclared;
}

void check_operands(AST *node)
{
    if(node == NULL) return;

    for(int i = 0; i < MAX_CHILD; i++){
        setNodeTypes(node->child[i]);
    }

    if(node->type == AST_SYMBOL){
        if(node->symbol->type == SYMBOL_VEC || node->symbol->type == SYMBOL_FUNC){
            fprintf(stderr, "Semantic ERROR in line %d: function/vector used as scalar variable.\n", node->lineNumber);
            SemanticErrors++;
        }
        node->datatype = node->symbol->datatype;
    }
    else if(node->type == AST_FUNC || node->type == AST_VEC){
        node->datatype = node->symbol->datatype;
    }
    else if(node->type == AST_PAREN){
        node->datatype = node->child[0]->datatype;
    }
    else if(isAritmeticOp(node->type)){
        AST* child0 = node->child[0];
        AST* child1 = node->child[1];
        if(!isDatatypeCompatible(child0->datatype, child1->datatype) || child0->datatype == DATATYPE_BOOL || child1->datatype == DATATYPE_BOOL){
            fprintf(stderr, "Semantic ERROR in line %d: Arithmetic operation with incompatible data types.\n", node->lineNumber);
            SemanticErrors++;
        }
        node->datatype = greaterDatatype(child0->datatype, child1->datatype);
    }
    else if(isRelationalOp(node->type)){
        if(!isNumerical(node->child[0]->datatype) || !isNumerical(node->child[1]->datatype)){
            fprintf(stderr, "Semantic ERROR in line %d: Relational operation with incompatible data types.\n", node->lineNumber);
            SemanticErrors++;
        }
        node->datatype = DATATYPE_BOOL;
    }
    else if(isLogicalOp(node->type)){
        if(node->type == AST_NOT){
            if(node->child[0]->datatype != DATATYPE_BOOL){
                fprintf(stderr, "Semantic ERROR in line %d: Logical operation with incompatible data types.\n", node->lineNumber);
                SemanticErrors++;
            }
        }
        else if(node->child[0]->datatype != DATATYPE_BOOL || node->child[1]->datatype != DATATYPE_BOOL){
            fprintf(stderr, "Semantic ERROR in line %d: Logical operation with incompatible data types.\n", node->lineNumber);
            SemanticErrors++;
        }
        node->datatype = DATATYPE_BOOL;
    }
}

int isDatatypeCompatible(int datatype1, int datatype2){
    return (isInteger(datatype1) && isInteger(datatype2)) || (datatype1 == datatype2);
}

int isInteger(int datatype){
    return (datatype == DATATYPE_CHAR || datatype == DATATYPE_INT);
}

int isNumerical(int datatype){
    return (datatype == DATATYPE_CHAR || datatype == DATATYPE_INT || datatype == DATATYPE_REAL);
}

int isRelationalOp(int nodetype){
    return (nodetype == AST_LESSER || nodetype == AST_GREATER || nodetype == AST_EQUAL || nodetype == AST_LESSOP || nodetype == AST_GREATOP || nodetype == AST_DIF);
}

int isAritmeticOp(int nodetype){
    return (nodetype == AST_ADD || nodetype == AST_SUB || nodetype == AST_MUL || nodetype == AST_DIV);
}

int isLogicalOp(int nodetype){
    return (nodetype == AST_AND || nodetype == AST_OR || nodetype == AST_NOT);
}

int greaterDatatype(int type1, int type2){
    return type1 > type2 ? type1 : type2;
}