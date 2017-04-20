CFLAGS = -g -DDEBUG
objects = y.tab.o node.o symtab.o codegen.o pfcodegen.o sqcodegen.o codegencheck.o

all: NESL2C

debug : $(objects)
	cc $(CFLAGS) -o NESL2C $(objects) 

NESL2C : $(objects)
	cc -o NESL2C $(objects)

y.tab.o: yacc.y lex.yy.o
	yacc yacc.y -d 
	cc -c y.tab.c -g

lex.yy.o: lex.l
	lex lex.l

#debug :
#	lex lex.l
#	yacc yacc.y -d --verbose --debug
#	gcc -c lex.yy.c -g
#	gcc -c y.tab.c -g 
#	gcc -c symtab.c -g 
#	gcc -c node.c -g 
#	gcc y.tab.o node.o symtab.o -g -ly -ll 
#
#run :
#	./a.out < test/sequence_good
##	vim output/NESL2C_test.c
#	gcc output/NESL2C_test.c
##	./a.out < ~/trunk/bench/NESL/muladd.nesl

clean :
	rm NESL2C $(objects)
