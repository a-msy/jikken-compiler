all: 
	flex verb.l
	gcc lex.yy.c -lfl -o verb.out
	./verb.out

flex:
	flex verb.l

gcc:
	gcc lex.yy.c -lfl -o verb.out

exe:
	./verb.out