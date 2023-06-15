#include <stdio.h>
#include <stdlib.h>

//lex.yy.h
int yylex();
extern char *yytext;
extern FILE *yyin;

int isRunning(void);
int getLineNumber(void);
void initMe(void);

int main(int argc, char **argv)
{
  FILE *file = 0;
  int token = 0;
  int answer = 0;
  int corrects = 0;
  int tokenNumber = 1;

  if (!argv[1])
  {
    fprintf(stdout, "\n No INPUTFILE \n Usage: ./etapa2 INPUTFILE \n");
    fprintf(stdout, "\nCompilation exit. CODE 1.\n");
    exit(1);
  }

  if (0 == (yyin = fopen(argv[1], "r")))
  {
    printf("Cannot open file %s... \n", argv[1]);
    fprintf(stdout, "\nCompilation exit. CODE 2.\n");
    exit(2);
  }

  initMe();
  yyparse();
  hashPrint();
  fprintf(stdout, "\n Compilation successful \n exiting...\n");
  fprintf(stdout, "\nCompilation exit. CODE 0.\n");
  exit(0);
}
