
#include "NotOr.h"
#include "Visitor.h"

using namespace nesl2c;

NotOr::NotOr(Node* a, Node* b) : LogicOperation(a, b)
{
}

void NotOr::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
