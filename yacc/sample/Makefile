all:
	flex sample.l
	bison -d sample.y
	gcc sample.tab.c lex.yy.c -o sample.out -lfl -ly
	./sample.out