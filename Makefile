CFLAGS = -g 
objects = y.tab.o node.o symtab.o codegen.o pfcodegen.o sqcodegen.o codegencheck.o treenode.o main.o

all: NESL2C

debug : $(objects)
	cc -DDEBUG $(CFLAGS) -o NESL2C $(objects) 

NESL2C : $(objects)
	cc -o NESL2C $(objects)

y.tab.o: nesl.y lex.yy.c
	yacc nesl.y -d 
	cc -c y.tab.c -g

lex.yy.c: nesl.l
	lex nesl.l

clean :
	rm NESL2C $(objects)
