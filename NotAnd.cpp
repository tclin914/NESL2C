
#include "NotAnd.h"
#include "Visitor.h"

using namespace nesl2c;

NotAnd::NotAnd(Node* a, Node* b) : LogicOperation(a, b)
{
}

void NotAnd::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
