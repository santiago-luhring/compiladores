// Código inspirado naquele apresentado nas video aulas
//      da disciplina INF01147 da UFRGS,
//      do Professor Marcelo Johann

#include "hash.h"

HASH_NODE *Table[HASH_SIZE];

// Inicia a tabela hash
void hashInit(void)
{
    int i;
    for (i = 0; i < HASH_SIZE; ++i)
    {
        Table[i] = 0;
    }
}

// Multiplica os caracteres e
// tira o resto de um primo para gerar um
// endereço quase aleatorio
int hashAddress(char *text)
{
    int address = 1;
    int i;
    for (i = 0; i < strlen(text); ++i)
    {
        address = (address * text[i]) % HASH_SIZE + 1;
    }
    return address - 1;
}

// Busca na lista encadeada do endereço
// pelo elemento enviado
HASH_NODE *hashFind(char *text)
{
    HASH_NODE *node;
    int address = hashAddress(text);
    for (node = Table[address]; node; node = node->next)
        if (strcmp(node->text, text) == 0)
        {
            return node;
        }
    return 0;
}

// Insere na tabela se o nó não existe ainda
HASH_NODE *hashInsert(char *text, int type)
{

    HASH_NODE *newnode;
    int address = hashAddress(text);
    if ((newnode = hashFind(text)) != 0)
    {
        return newnode;
    }
    newnode = (HASH_NODE *)calloc(1, sizeof(HASH_NODE));
    newnode->type = type;

    if(newnode->type == SYMBOL_LIT_CHAR) newnode->datatype = DATATYPE_INT;
    else if(newnode->type == SYMBOL_LIT_INT) newnode->datatype = DATATYPE_INT;
    else if(newnode->type == SYMBOL_LIT_REAL) newnode->datatype = DATATYPE_REAL;

    newnode->text = (char *)calloc(strlen(text) + 1, sizeof(char));
    strcpy(newnode->text, text);
    newnode->next = Table[address];
    Table[address] = newnode;
    return newnode;
}

// Imprime a tabela
void hashPrint(void)
{
    int i;
    HASH_NODE *node;
    for(int i = 0; i < HASH_SIZE; i++){
        if(Table[i] != NULL){
            for(node = Table[i]; node != NULL; node = node->next){
                fprintf(stderr, "Table[%d] - type: %d datatype: %d text: %s\n", i, node->type, node->datatype, node->text);
            }
        }
    }
}

int hash_check_undeclared(void)
{
	HASH_NODE * node;
	int undeclaredVariables = 0;
	for(int i = 0; i < HASH_SIZE; i++){
		if(Table[i] != NULL){
			for(node = Table[i]; node != NULL; node = node->next){
				if(node->type == SYMBOL_IDENTIFIER){
					fprintf(stderr, "Semantic ERROR: Variable %s not declared.\n", node->text);				
					undeclaredVariables++;
				}
			}
		}
	}
	return undeclaredVariables;
}

HASH_NODE* makeTemp(int datatype){
	static int serialNumber = 0;
	static char buffer[256];
	sprintf(buffer, "_temp%d", serialNumber++);
    HASH_NODE *node = hashInsert(buffer, SYMBOL_VAR);  
    if(datatype){
        node->datatype = datatype;
    }
	return node;
}

HASH_NODE* makeLabel(){
	static int serialNumber = 0;
	static char buffer[128];
	sprintf(buffer, "_label%d", serialNumber++);
	return hashInsert(buffer, SYMBOL_LABEL);
}
HASH_NODE** getTable(){
    return Table;
}