#ifndef ASM_H
#define ASM_H

#include "tacs.h"
#include "treeAST.h"

void addTemporaries(FILE* out);

void addImmediates(FILE* out);

void addData(FILE* out, AST* node);

void asmGenerate(TAC *firstTac, AST *ast);


#endif