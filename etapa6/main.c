#include <stdio.h>
#include <stdlib.h>
#include "hash.h"
#include "treeAST.h"
#include "tacs.h"
#include "asm.h"

//lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;
extern FILE *inputfile();
int isRunning(void);
int getLineNumber(void);
void initMe(void);
int yyparse();
extern AST *getAST();
extern int checkSemantic();
extern int semanticErrors;
extern TAC* getTACs();

int main(int argc, char *argv[])
{
  FILE *outputfile;


  if(argc < 3)
  {
    fprintf(stderr, "\n No INPUTFILE or OUTPUTFILE \n Usage: ./etapa3 INPUTFILE OUTPUTFILE\n");
    fprintf(stderr, "\nCompilation exit. CODE 1.\n");
    exit(1);
  }

  if(!(inputfile(argv[1])))
  {
    fprintf(stderr,"Cannot open file... %s\n", argv[1]);
    fprintf(stderr, "\nCompilation exit. CODE 2.\n");
    exit(2);
  }

  if(!(outputfile = fopen(argv[2], "w+"))){
		fprintf(stderr,"Cannot open file... %s\n",argv[2]);
		fprintf(stderr,"\nCompilation exit, CODE 2.");
    exit(2);
	}

  initMe();
  yyparse();
  //hashPrint();
  fprintf(stderr, "\n Compilation successful.\n");
  fprintf(stderr,"\n Starting Decompilation \n");
  decompileAST(getAST(),outputfile);
  fclose(outputfile);
  asmGenerate(getTACs(), getAST());
  //getTACS();
  //checkSemantic();


  exit(0);
}
