#include "function_hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

function_hash_node* FunTable[FUNCTION_HASH_SIZE];

void function_hashInit(void){
    for(int i = 0; i < FUNCTION_HASH_SIZE; i++){
        FunTable[i] = 0;
    }
}

function_hash_node* function_hashInsert(char* name){

  function_hash_node * newnode;
    int address = function_hashAddress(name);
    newnode = (function_hash_node*) calloc(1, sizeof(function_hash_node));

    newnode->name = calloc(strlen(name) + 1, sizeof(char));
    strcpy(newnode->name, name);

    newnode->first_argument = NULL;
    FunTable[address] = newnode;

    return newnode;
}

function_hash_node * getNode(char * name){
  return FunTable[function_hashAddress(name)];
}

void addNewArgument(char * argName, function_hash_node* node){
  function_argument * arg = node->first_argument;
  if(arg == NULL) {
    node->first_argument = (function_argument*) malloc(sizeof(function_argument));
    arg = node->first_argument;
  }else{
    while(arg->next != NULL){
      arg = arg->next;
    }
    arg->next = (function_argument*) malloc(sizeof(function_argument));
    arg = arg->next;
  }
  arg->next = NULL;
  arg->name = calloc(strlen(argName) + 1, sizeof(char));
  strcpy(arg->name, argName);
}

int function_hashAddress(char *text){
    int address = 1;
    for(int i = 0; i < strlen(text); i++){
        address = (address * text[i]) % FUNCTION_HASH_SIZE + 1;
    }
    return address - 1;
}
function_hash_node** getFunTable(){
    return FunTable;
}
