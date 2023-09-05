// Referência:
// Código majoritariamente retirado das video aulas 
//      da disciplina INF01147 da UFRGS,
//      do Professor Marcelo Johann 

#ifndef HASH_HEADER

#define HASH_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 4999

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_REAL 2
#define SYMBOL_LIT_CHAR 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_IDENTIFIER 5

#define SYMBOL_LABEL 6

#define SYMBOL_VAR 10
#define SYMBOL_VEC 11
#define SYMBOL_FUNC 12
#define SYMBOL_PARAM 13

#define DATATYPE_CHAR 14
#define DATATYPE_INT 15
#define DATATYPE_REAL 16
#define DATATYPE_BOOL 17

typedef struct hash_node
{
    int type;
    int datatype;
    char *text;
    struct hash_node *next;
} HASH_NODE;

void hashInit(void);
int hashAddress(char *text);
HASH_NODE *hashFind(char *text);
HASH_NODE *hashInsert(char *text, int type);
void hashPrint(void);
int hash_check_undeclared();

HASH_NODE* makeTemp();
HASH_NODE* makeLabel();
HASH_NODE** getTable();

#endif
