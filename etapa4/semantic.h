#include "hash.h"
#include "treeAST.h"

#ifndef SEMANTIC_HEADER
#define SEMANTIC_HEADER
#endif

extern int SemanticErrors;

void check_and_set_declaration(AST *node);
void check_undeclared();
void check_operands(AST *node);