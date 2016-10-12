all :
	lex lex.l
	yacc yacc.y -d
	gcc -c lex.yy.c
	gcc -c y.tab.c
	gcc lex.yy.o -ll -ly

run :
	./a.out < ~/trunk/bench/NESL/muladd.nesl

clean :
	rm lex.yy.c lex.yy.o y.tab.o y.tab.h y.tab.c 
