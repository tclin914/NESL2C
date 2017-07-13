
#include "Or.h"
#include "Visitor.h"

using namespace nesl2c;

Or::Or(Node* a, Node* b) : LogicOperation(a, b)
{
}

void Or::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
