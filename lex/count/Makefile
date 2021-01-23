all: 
	flex count.l
	gcc lex.yy.c -lfl -o count.out
	./count.out

flex:
	flex count.l

gcc:
	gcc lex.yy.c -lfl -o count.out

exe:
	./count.out