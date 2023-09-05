#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for single compiler call
# All source files must be included from code embedded in scanner.l
# In our case, you probably need #include "hash.c" at the beginning
# and #include "main.c" in the last part of the scanner.l
#

etapa6: main.o lex.yy.o y.tab.o hash.o treeAST.o semantic.o tacs.o  function_hash.o asm.o
	gcc main.o lex.yy.o y.tab.o hash.o treeAST.o semantic.o tacs.o  function_hash.o asm.o -o etapa6

main.o: main.c
	gcc -c main.c

semantic.o: semantic.c
	gcc -c semantic.c

hash.o: hash.c
	gcc -c hash.c

function_hash.o: function_hash.c
	gcc -c function_hash.c

asm.o: asm.c
	gcc -c asm.c

tacs.o: tacs.c
	gcc -c tacs.c

lex.yy.o: lex.yy.c y.tab.o
	gcc -c lex.yy.c

y.tab.o: y.tab.c
	gcc -c y.tab.c


treeAST.o: treeAST.c
	gcc -c treeAST.c

lex.yy.c: scanner.l
	lex scanner.l

y.tab.c: parser.y
	yacc -d parser.y

clean:
	rm lex.yy.c y.tab.c y.tab.h *.o etapa6