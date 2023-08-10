#include "hash.h"
#include "treeAST.h"
#include "stdbool.h"

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER
#endif

extern int SemanticErrors;

int semanticVerification(AST *root);

void check_and_set_declaration(AST *node);
void set_declaration(AST *node, int symbol);
void check_undeclared();
void check_operands(AST *node);

//aux
int isDatatypeCompatible(int datatype1, int datatype2);
int isInteger(int datatype);
int isNumerical(int datatype);
int isRelationalOp(int nodetype);
int isAritmeticOp(int nodetype);
int isLogicalOp(int nodetype);
int greaterDatatype(int type1, int type2);
bool check_vec_elements(AST *node, int datatype);
void validateFunction(AST * node);
void checkPrint(AST *node);
AST *findFunctionDeclaration(char * name, AST * node);
bool checkNumberOfArguments(AST * node, AST * declaration);
int getNumberOfArguments(AST * node);
void compareCalledArguments(AST * node, AST * declaration);
void isReturnCompatible(AST * node, int datatype);
void checkReturns(AST *node);
