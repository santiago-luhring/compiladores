#ifndef TACS HEADER
#define TACS_HEADER

#include "hash.h"
#include "treeAST.h"

#define TAC_SYMBOL 0
#define TAC_COPY 1
#define TAC_ADD 2
#define TAC_SUB 3
#define TAC_MUL 4
#define TAC_DIV 5
#define TAC_GREATER 6
#define TAC_LESSER 7
#define TAC_GREATOP 8
#define TAC_LESSOP 9
#define TAC_EQUAL 10
#define TAC_DIF 11
#define TAC_AND 12
#define TAC_OR 13
#define TAC_NOT 14

#define TAC_LABEL 15
#define TAC_BEGINFUN 16
#define TAC_ARGPUSH 17
#define TAC_ENDFUN 18
#define TAC_IFZ 19
#define TAC_JUMP 20
#define TAC_CALL 21
#define TAC_ARG 22
#define TAC_RET 23
#define TAC_PRINT 24
#define TAC_READ 25
#define TAC_VECATTR 26
#define TAC_VEC 27
#define TAC_PARAMPOP 28

typedef struct tac_struct{
	int type;
	HASH_NODE *res;
	HASH_NODE *op1;
	HASH_NODE *op2;
	struct tac_struct *prev;
	struct tac_struct *next; 
} TAC;


TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);
TAC* tacJoin(TAC* l1, TAC* l2);
TAC* createBinop(int type, TAC* child[]);

void tacPrint(TAC* tac);
void tacPrintAll(TAC* tac);

TAC* generateCode(AST* node);

#endif