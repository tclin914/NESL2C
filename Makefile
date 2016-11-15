all :
	lex lex.l
	yacc yacc.y -d
	gcc -c lex.yy.c
	gcc -c y.tab.c
	gcc -c symtab.c
	gcc -c node.c
	gcc y.tab.o node.o symtab.o -ly -ll 

debug :
	lex lex.l
	yacc yacc.y -d --verbose --debug
	gcc -c lex.yy.c -g
	gcc -c y.tab.c -g 
	gcc -c symtab.c -g 
	gcc -c node.c -g 
	gcc y.tab.o node.o symtab.o -g -ly -ll 

newtest :
	lex newlex.l
	yacc newyacc.y -d --verbose --debug
	gcc -c lex.yy.c -g
	gcc -c y.tab.c -g 
	gcc -c symtab.c -g 
	gcc -c newnode.c -g 
	gcc y.tab.o node.o symtab.o -g -ly -ll 


run :
	./a.out < test/sequence_good
#	vim output/NESL2C_test.c
	gcc output/NESL2C_test.c
#	./a.out < ~/trunk/bench/NESL/muladd.nesl

clean :
	rm lex.yy.c lex.yy.o y.tab.o y.tab.h y.tab.c 
