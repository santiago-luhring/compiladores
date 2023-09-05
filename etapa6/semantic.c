#include "semantic.h"

int SemanticErrors = 0;
AST *ROOT;
bool etapa4 = false;
int semanticVerification(AST *root)
{
    if(etapa4){
        ROOT = root;
        check_and_set_declaration(root);
        check_undeclared();
        check_operands(root);    
        return SemanticErrors;
    }
    else
        return 0;
}

void check_and_set_declaration(AST *node)
{
    if(node == NULL)
        return;

    switch (node->type)
    {
    case AST_DECVAR:
        set_declaration(node, SYMBOL_VAR);
        break;
    case AST_DECFUNC:
        set_declaration(node, SYMBOL_FUNC);
        break;
    case AST_DECVEC:
        set_declaration(node, SYMBOL_VEC);
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
        SemanticErrors++;
    }
    else
    {
        node->symbol->type = symbol;
        if(node->child[0]->type == AST_TCHAR) node->symbol->datatype = DATATYPE_CHAR;
        else if(node->child[0]->type == AST_TINT) node->symbol->datatype = DATATYPE_INT;
        else if(node->child[0]->type == AST_TREAL) node->symbol->datatype = DATATYPE_REAL;
    }
    if(symbol == SYMBOL_VAR)
    {
        if(!isDatatypeCompatible(node->symbol->datatype, node->child[1]->symbol->datatype))
        {
            fprintf(stderr, "Semantic ERROR in line %d: Variable declaration with mixed dataypes\n", node->lineNumber);
            SemanticErrors++;
        }
    }
    if (symbol == SYMBOL_VEC)
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
        check_operands(node->child[i]);
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

bool check_vec_elements(AST *node, int datatype)
{
    if(node != NULL)
    {
        if(!isDatatypeCompatible(node->child[0]->symbol->datatype, datatype))
            return false;
        if(node->child[1] != NULL)
            return check_vec_elements(node->child[1], datatype);
        
    }
    return true;
}
void isReturnCompatible(AST *node, int datatype){
	if(node == NULL) return;
	if(node->type == AST_RETURN){
		if(!isDatatypeCompatible(node->child[0]->datatype, datatype)){
			printf("Semantic ERROR in line %d: Return statement with wrong datatype.\n", node->lineNumber);
			SemanticErrors++;
		}
	}
	for(int i = 0; i < MAX_CHILD; i++){
		isReturnCompatible(node->child[i], datatype);
	}
}

void checkReturns(AST * node){
	if(node != NULL && node->type == AST_DECFUNC){
		isReturnCompatible(node, node->symbol->datatype);
	}

	for(int i = 0; i < MAX_CHILD; i++){
		if(node->child[i] == NULL)
			break;
		checkReturns(node->child[i]);
	}
}

void checkPrint(AST *node){
    if (node == NULL) return;

    if(node->child[0]->type == AST_SYMBOL){
        if(node->child[0]->symbol->type == SYMBOL_FUNC){
            fprintf(stderr, "Semantic ERROR in line %d: Cannot print function\n", node->lineNumber);
            SemanticErrors++;
        }
        else if(node->child[0]->symbol->type == SYMBOL_VEC){
            fprintf(stderr, "Semantic ERROR in line %d: Cannot print vector\n", node->lineNumber);
            SemanticErrors++;
        }
    }
    checkPrint(node->child[1]);

}

void validateFunction(AST *node){
	AST* declaration = findFunctionDeclaration(node->symbol->text, ROOT);
    if(declaration == NULL){
        fprintf(stderr, "Semantic ERROR in line %d: Only functions can be called.\n", node->lineNumber);
        SemanticErrors++;
    }
	else if(checkNumberOfArguments(node, declaration)){
		compareCalledArguments(node->child[0], declaration->child[1]);					
	}
}

bool checkNumberOfArguments(AST *node, AST *declaration){
	int numberOfCalledArguments = getNumberOfArguments(node->child[0]);
	int numberOfDeclaredArguments = getNumberOfArguments(declaration->child[1]);	
	if(numberOfCalledArguments != numberOfDeclaredArguments){
    	fprintf(stderr, "Semantic ERROR in line %d: Incompatible number of arguments.\n", node->lineNumber);
		SemanticErrors++;
		return false;
	}
	return true;
}

AST* findFunctionDeclaration(char * name, AST * node){
	if(node->symbol != NULL && node->type == AST_DECFUNC && strcmp(node->symbol->text, name) == 0)
		return node;

	for(int i = 0; i < MAX_CHILD; i++){
		if(node->child[i] == NULL)
			return NULL;
		AST * finding = findFunctionDeclaration(name, node->child[i]) ;
		if(finding != NULL)
			return finding;
	}
	return NULL;
}

int getNumberOfArguments(AST * node){
    if(node == NULL) return 0;
	if(node->child[1] != NULL)
		return 1 + getNumberOfArguments(node->child[1]);
	else
		return 0;
}

void compareCalledArguments(AST *node, AST *declaration){
	if(node->child[0] != NULL){
		if(!isDatatypeCompatible(node->child[0]->datatype, declaration->child[0]->symbol->datatype)){
			fprintf(stderr, "Semantic ERROR in line %d: Incompatible argument types\n", node->lineNumber);
			SemanticErrors++;
		}
        if(node->child[0]->type == AST_SYMBOL){
            if(node->child[0]->symbol->type == SYMBOL_FUNC){
                fprintf(stderr, "Semantic ERROR in line %d: Cannot pass function as argument\n", node->lineNumber);
			    SemanticErrors++;
            }
            else if(node->child[0]->symbol->type == SYMBOL_VEC){
                fprintf(stderr, "Semantic ERROR in line %d: Cannot pass vector as argument\n", node->lineNumber);
			    SemanticErrors++;
            }
        }
		if(node->child[1] != NULL)
			compareCalledArguments(node->child[1], declaration->child[1]);
	}
}

void checkUsage(AST *node){
    if(node == NULL) return;
	switch (node->type){
        case AST_ATTRIB:
            if(node->symbol->type != SYMBOL_VAR){
                fprintf(stderr, "Semantic ERROR in line %d: Attribution must be to a scalar variable.\n", node->lineNumber);
                SemanticErrors++;
            }
            if(!isDatatypeCompatible(node->symbol->datatype, node->child[0]->datatype)){
                fprintf(stderr, "Semantic ERROR in line %d: Attribution with incompatible data type.\n", node->lineNumber);
                SemanticErrors++;
            }
            break;
        case AST_VECATTR:
            if(node->symbol->type != SYMBOL_VEC){
                fprintf(stderr, "Semantic ERROR in line %d: Indexing only allowed for vectors.\n", node->lineNumber);
                SemanticErrors++;
            }
            if(!isDatatypeCompatible(node->symbol->datatype, node->child[1]->datatype)){
                fprintf(stderr, "Semantic ERROR in line %d: Attribution with incompatible data type.\n", node->lineNumber);
                SemanticErrors++;
            }
            if(!isInteger(node->child[0]->datatype)){
                fprintf(stderr, "Semantic ERROR in line %d: Index must be an integer.\n", node->lineNumber);
                SemanticErrors++;
            }
            break;
        case AST_FUNC:
			validateFunction(node);
			break;
        case AST_INPUT:
            if(node->symbol->type != SYMBOL_VAR){
				fprintf(stderr, "Semantic ERROR in line %d: read only allowed to scalar variables.\n", node->lineNumber);
				SemanticErrors++;
			}
            break;
        case AST_OUTPUT:
            checkPrint(node->child[0]);
            break;
        case AST_IF:
        case AST_IFELSE:
        case AST_IFELWHILE:
            if(node->child[0]->datatype != DATATYPE_BOOL){
                fprintf(stderr, "Semantic ERROR in line %d: Condition must be a boolean expression.\n", node->lineNumber);
				SemanticErrors++;
            }
            break;
        default:
            break;
    }

    for(int i = 0; i < MAX_CHILD; i++)
		checkUsage(node->child[i]);
}