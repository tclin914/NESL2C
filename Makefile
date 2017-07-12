
objects = lex.yy.o nesl.tab.o Main.o Node.o NullaryNode.o ConstantBoolean.o         \
					ConstantInteger.o ConstantFloat.o ConstantString.o Identifier.o						\
					TypeNode.o BinaryNode.o Assign.o Let.o ApplyToEach.o ApplyBody.o          \
					FuncCall.o Sequence.o                                                     \
					ArithmeticOpFactory.o ArithmeticOperation.o Add.o Subtract.o PP.o         \
					LARROW.o Mul.o Div.o RARROW.o RelationalOpFactory.o RelationalOperation.o \
					Equal.o NotEqual.o LessThan.o GreaterThan.o LessEqual.o GreaterEqual.o    \
					LogicOpFactory.o LogicOperation.o And.o NotAnd.o Or.o NotOr.o XOr.o       \
					UnaryNode.o EmptySequence.o UnaryOpFactory.o UnaryOperation.o Sharp.o     \
					At.o Uminus.o In.o SequenceTail.o TernaryNode.o IfElse.o

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
