
#include "nesl2c/AST/Div.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Div::Div(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void Div::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
