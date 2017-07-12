
objects = lex.yy.o nesl.tab.o Main.o Node.o NullaryNode.o ConstantBoolean.o         \
					ConstantInteger.o ConstantFloat.o ConstantString.o BinaryNode.o           \
					ArithmeticOpFactory.o ArithmeticOperation.o Add.o Subtract.o PP.o         \
					LARROW.o Mul.o Div.o RARROW.o RelationalOpFactory.o RelationalOperation.o \
					Equal.o NotEqual.o LessThan.o GreaterThan.o LessEqual.o GreaterEqual.o    

all: NESL2C

NESL2C : $(objects)
	$(CXX) -o NESL2C $(objects)

lex.yy.o: lex.yy.c nesl.tab.h
	$(CXX) -c lex.yy.c 

nesl.tab.o: nesl.tab.c
	$(CXX) -c nesl.tab.c

lex.yy.c: nesl.l nesl.tab.h
	flex nesl.l

nesl.tab.c nesl.tab.h: nesl.y 
	bison -d nesl.y  

clean :
	rm -rf NESL2C $(objects) nesl.tab.c nesl.tab.h lex.yy.c
