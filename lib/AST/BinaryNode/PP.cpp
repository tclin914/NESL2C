
#include "nesl2c/AST/PP.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

PP::PP(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void PP::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
