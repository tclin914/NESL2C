
#include "nesl2c/AST/Subtract.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Subtract::Subtract(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Subtract::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
