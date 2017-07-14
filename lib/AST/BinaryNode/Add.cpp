
#include "nesl2c/AST/Add.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Add::Add(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Add::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
