
#include "XOr.h"
#include "Visitor.h"

using namespace nesl2c;

XOr::XOr(Node* a, Node* b) : LogicOperation(a, b)
{
}

void XOr::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
