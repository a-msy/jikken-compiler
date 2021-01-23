# How to Use :: make all N=filename
all: 
	flex comp.l
	bison -d comp.y
	gcc comp.tab.c lex.yy.c ast.c -o comp.out -DYYERROR_VERBOSE -lfl -ly
	./comp.out kadai${N} < kadai${N}

compile:
	flex -d comp.l
	bison comp.y
	gcc comp.tab.c lex.yy.c ast.c -o comp.out -DYYERROR_VERBOSE -lfl -ly

test:
	make compile
	./comp.out kadai1 < kadai1
	./comp.out kadai2 < kadai2
	./comp.out kadai3 < kadai3
	maps -e -q kadai1.s
	maps -e -q kadai2.s
	maps -e -q kadai3.s

compileg:
	flex -d comp.l
	bison comp.y
	gcc comp.tab.c lex.yy.c ast.c -o comp.out -DYYERROR_VERBOSE -lfl -ly -g
	./comp.out kadai${N} < kadai${N}

debug:
	gdb --args comp.out --kadai${N} 

line:
	addr2line -e comp.out ${L}
