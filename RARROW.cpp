
#include "RARROW.h"
#include "Visitor.h"

using namespace nesl2c;

RARROW::RARROW(Node* a, Node* b) : ArithmeticOperation(a, b)
{
}

void RARROW::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
