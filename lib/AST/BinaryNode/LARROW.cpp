
#include "nesl2c/AST/LARROW.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

LARROW::LARROW(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void LARROW::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
