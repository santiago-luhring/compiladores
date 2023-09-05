#ifndef FUNCTION_HASH_H
#define FUNCTION_HASH_H

#define FUNCTION_HASH_SIZE 997

#define SYMBOL_LIT_INT 1
#define SYMBOL_LIT_FLOAT 2
#define SYMBOL_LIT_CHAR 3
#define SYMBOL_LIT_STRING 4
#define SYMBOL_IDENTIFIER 5

#define SYMBOL_LABEL 6

#define SYMBOL_VAR 10
#define SYMBOL_VEC 11
#define SYMBOL_FUNC 12
#define SYMBOL_PARAM 13

#define DATATYPE_BYTE 14
#define DATATYPE_INT 15
#define DATATYPE_FLOAT 16
#define DATATYPE_BOOL 17

typedef struct function_argument{
  char * name;
  struct function_argument * next;
}function_argument;

typedef struct function_hash_node{
    int type;
    int datatype;
  char * name;
  function_argument* first_argument;
} function_hash_node;


void function_hashInit(void);

function_hash_node* function_hashInsert(char* name);

int function_hashAddress(char *text);

function_hash_node** getFunTable();
function_hash_node * getNode(char * name);
void addNewArgument(char * argName, function_hash_node* node);

#endif
