
#include "nesl2c/AST/Mul.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Mul::Mul(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Mul::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
