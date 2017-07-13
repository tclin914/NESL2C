
#include "Mul.h"
#include "Visitor.h"

using namespace nesl2c;

Mul::Mul(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Mul::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
