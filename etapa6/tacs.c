#include "tacs.h"



TAC* tacCreate(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2)
{
    TAC* tacCreate = NULL;
    tacCreate = (TAC*) calloc(1, sizeof(TAC));
    tacCreate->type = type;
    tacCreate->res = res;
    tacCreate->op1 = op1;
    tacCreate->op2 = op2;
    tacCreate->next = NULL;
    tacCreate->prev = NULL;

    return tacCreate;
}

TAC* tacJoin(TAC* l1, TAC* l2)
{
    TAC* point;
    if(!l1)return l2;
    if(!l2)return l1;
    point = l2;
	while(point->prev){
		point = point->prev;	
	}
	point->prev = l1;
	return l2;
}


void tacPrint(TAC* tac)
{
	if(!tac) return;
	if(tac->type == TAC_SYMBOL) return;
	
	fprintf(stderr, "TAC(");
	switch(tac->type){
		case TAC_COPY: fprintf(stderr, "COPY(");break;
		case TAC_ADD: fprintf(stderr, "ADD(");break;
		case TAC_SUB: fprintf(stderr, "SUB(");break;
		case TAC_MUL: fprintf(stderr, "MUL(");break;
		case TAC_DIV: fprintf(stderr, "DIV(");break;
		case TAC_GREATER: fprintf(stderr, "GREATER(");break;
		case TAC_LESSER: fprintf(stderr, "LESSER(");break;
		case TAC_GREATOP: fprintf(stderr, "GREATOP(");break;
		case TAC_LESSOP: fprintf(stderr, "LESSOP(");break;
		case TAC_EQUAL: fprintf(stderr, "EQUAL(");break;
		case TAC_DIF: fprintf(stderr, "DIF(");break;
		case TAC_AND: fprintf(stderr, "AND(");break;
		case TAC_OR: fprintf(stderr, "OR(");break;
		case TAC_NOT: fprintf(stderr, "NOT(");break;
		case TAC_LABEL: fprintf(stderr, "LABEL(");break;
		case TAC_BEGINFUN: fprintf(stderr, "BEGINFUN(");break;
		case TAC_ARGPUSH: fprintf(stderr, "ARGPUSH(");break;
		case TAC_ENDFUN: fprintf(stderr, "ENDFUN(");break;
		case TAC_IFZ: fprintf(stderr, "IFZ(");break;
		case TAC_JUMP: fprintf(stderr, "JUMP(");break;
		case TAC_CALL: fprintf(stderr, "CALL(");break;
		case TAC_ARG: fprintf(stderr, "ARG(");break;
		case TAC_RET: fprintf(stderr, "RET(");break;
		case TAC_PRINT: fprintf(stderr, "OUTPUT(");break;
		case TAC_READ: fprintf(stderr, "INPUT(");break;
		case TAC_VECATTR: fprintf(stderr, "VECATTR(");break;
		case TAC_VEC: fprintf(stderr, "VEC(");break;
	

		default: fprintf(stderr, "UNKNOWN TAC TYPE!(");break;
	}

	if(tac->res)
		fprintf(stderr, "%s, ", tac->res->text);
	else
		fprintf(stderr, "0, ");

	if(tac->op1)
		fprintf(stderr, "%s, ", tac->op1->text);
	else
		fprintf(stderr, "0, ");

	if(tac->op2)
		fprintf(stderr, "%s", tac->op2->text);
	else
		fprintf(stderr, "0");

	fprintf(stderr, "))\n");
}
void tacPrintAll(TAC* tac)
{
    if(!tac)return;
	else{
        tacPrintAll(tac->prev);
        tacPrint(tac);
    }
}

TAC* tacsReverse(TAC* l){
	TAC* t;
	for(t = l; t->prev; t = t->prev)
		t->prev->next = t;
	return t;
}

TAC* generateCode(AST* node)
{
	if(!node){
		return NULL;
	}
    TAC* result = 0;
	TAC* child[MAX_CHILD];
	
	for(int i = 0; i < MAX_CHILD; i++)
		child[i] = generateCode(node->child[i]);
	switch(node->type){
		case AST_SYMBOL: 	result = tacCreate(TAC_SYMBOL, node->symbol, 0, 0);break;
        case AST_ADD: 		result = createBinop(TAC_ADD, child);break;    
		case AST_SUB:		result = createBinop(TAC_SUB, child);break;
		case AST_MUL:		result = createBinop(TAC_MUL, child);break;
		case AST_DIV: 		result = createBinop(TAC_DIV, child);break;
		case AST_GREATER: 	result = createBinop(TAC_GREATER, child);break;
		case AST_LESSER: 	result = createBinop(TAC_LESSER, child);break;
		case AST_EQUAL: 	result = createBinop(TAC_EQUAL, child);break;
		case AST_GREATOP: 	result = createBinop(TAC_GREATOP, child);break;
		case AST_LESSOP:	result = createBinop(TAC_LESSOP, child);break;
		case AST_DIF: 		result = createBinop(TAC_DIF, child);break;
		case AST_AND: 		result = createBinop(TAC_AND, child);break;
		case AST_OR: 		result = createBinop(TAC_OR, child);break;
		case AST_NOT: 		result = createBinop(TAC_NOT, child);break;

        case AST_ATTRIB: 	result = tacJoin(child[0], tacCreate(TAC_COPY, node->symbol, child[0]?child[0]->res:0, 0));break;
		case AST_VECATTR: 	result = tacJoin(child[0], tacJoin(child[1], tacCreate(TAC_VECATTR, node->symbol, child[0]?child[0]->res:0, child[1]?child[1]->res:0)));break; 
		case AST_INPUT: 	result = tacCreate(TAC_READ, makeTemp(), 0, 0);break;
		case AST_OUTPUTL:
		case AST_NEXTOUT: 	result = tacJoin(tacJoin(child[0], tacCreate(TAC_PRINT, child[0]?child[0]->res:0, 0, 0)), child[1]);break;
		case AST_RETURN: 	result = tacJoin(child[0], tacCreate(TAC_RET, child[0]?child[0]->res:0, 0, 0));break;
		case AST_IFELSE:
		case AST_IF: 		result = createIf(child); break;
        case AST_IFELWHILE: break;
		
		case AST_FUNC: 		result = tacJoin(child[0], tacCreate(TAC_CALL, makeTemp(), node->symbol, 0));break;
		case AST_ARGL:
		case AST_NEXTARG: 	result = tacJoin(child[1], tacJoin(child[0], tacCreate(TAC_ARGPUSH, child[0]?child[0]->res:0, 0, 0)));break;
		case AST_VEC: 		result = tacJoin(child[0], tacCreate(TAC_VEC, makeTemp(), node->symbol, child[0]?child[0]->res:0));break;

		case AST_DECFUNC: 	result = createFunction(tacCreate(TAC_SYMBOL, node->symbol, 0, 0), child[1], child[2]);break;
		       

        default:			result = tacJoin(child[0],tacJoin(child[1],tacJoin(child[2], child[3])));break;
    }

    return result;
}

TAC* createBinop(int type, TAC* child[])
{
	HASH_NODE * op1;
	HASH_NODE * op2;
	if(child[0]) op1 = child[0]->res; else op1 = 0;
	if(child[1]) op2 = child[1]->res; else op2 = 0;
	return tacJoin(child[0], tacJoin(child[1], tacCreate(type, makeTemp(), op1, op2)));
}
TAC* createIf(TAC* child[])
{
	HASH_NODE* ifLabel = makeLabel();
	TAC* ifTac = tacJoin(child[0], tacCreate(TAC_IFZ, ifLabel, child[0]?child[0]->res:0, 0));
	TAC* ifLabelTac = tacCreate(TAC_LABEL, ifLabel, 0, 0);
	
	if(child[2]){
		HASH_NODE* elseLabel = makeLabel();
		TAC* elseLabelTac = tacCreate(TAC_LABEL, elseLabel, 0, 0);
		TAC* elseJumpTac = tacCreate(TAC_JUMP, elseLabel, 0, 0);
		TAC* ifElseTac = tacJoin(tacJoin(tacJoin(tacJoin(tacJoin(ifTac, child[1]), elseJumpTac), ifLabelTac), child[2]), elseLabelTac);
		return ifElseTac;
	}
    else{
		return tacJoin(tacJoin(ifTac, child[1]), ifLabelTac);
	}
}

TAC* createFunction(TAC* symbol, TAC* params, TAC* code){
	return tacJoin(tacJoin(tacJoin(tacCreate(TAC_BEGINFUN, symbol->res, 0, 0), params), code), tacCreate(TAC_ENDFUN, symbol->res, 0, 0));
}