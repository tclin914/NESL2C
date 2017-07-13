
#include "Subtract.h"
#include "Visitor.h"

using namespace nesl2c;

Subtract::Subtract(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Subtract::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
