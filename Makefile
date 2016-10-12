all :
	yacc yacc.y -d
	lex lex.l
	gcc y.tab.c

run :
	./a.out < ~/trunk/bench/NESL/muladd.nesl

clean :
	rm lex.yy.c lex.yy.o y.tab.o y.tab.h y.tab.c 
