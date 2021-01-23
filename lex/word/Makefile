all: 
	flex word.l
	gcc lex.yy.c -lfl -o word.out
	./word.out

flex:
	flex word.l

gcc:
	gcc lex.yy.c -lfl -o word.out

exe:
	./word.out