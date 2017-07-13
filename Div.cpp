
#include "Div.h"
#include "Visitor.h"

using namespace nesl2c;

Div::Div(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Div::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
