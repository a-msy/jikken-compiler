# How to Use :: make all N=num
run: 
	make compile
	./comp.out kadai${N} < kadai${N}

maps:
	make run N=${N}
	maps -e -q kadai${N}.s
	
compile:
	flex -d comp.l
	bison -d comp.y
	gcc comp.tab.c lex.yy.c ast.c -o comp.out -DYYERROR_VERBOSE -lfl -ly

test:
	make compile
	./comp.out kadai1 < kadai1
	./comp.out kadai2 < kadai2
	./comp.out kadai3 < kadai3
	maps -e -q kadai1.s
	maps -e -q kadai2.s
	maps -e -q kadai3.s

check:
	make compile
	./comp.out test < test.txt
	maps -e -q test.s
	
rung:
	flex -d comp.l
	bison -d comp.y
	gcc comp.tab.c lex.yy.c ast.c -o comp.out -DYYERROR_VERBOSE -lfl -ly -g
	./comp.out kadai${N} < kadai${N}

debug:
	gdb --args comp.out --kadai${N} 

line:
	addr2line -e comp.out ${L}
