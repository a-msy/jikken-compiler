# How to Use :: make all N=filename
all: 
	flex ${N}.l
	bison -d ${N}.y
	gcc ${N}.tab.c lex.yy.c -o ${N}.out -lfl -ly
	./${N}.out