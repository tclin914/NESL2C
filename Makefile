all :
	lex lex.l
	yacc yacc.y -d
	gcc -c lex.yy.c
	gcc -c y.tab.c
	gcc y.tab.o -ly -ll 

debug :
	lex lex.l
	yacc yacc.y -d --debug --verbose
	gcc -c lex.yy.c -g
	gcc -c y.tab.c -g 
	gcc y.tab.o -ly -ll

run :
	./a.out < ~/trunk/bench/NESL/muladd.nesl

clean :
	rm lex.yy.c lex.yy.o y.tab.o y.tab.h y.tab.c 
