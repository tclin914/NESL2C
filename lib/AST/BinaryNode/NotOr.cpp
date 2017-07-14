
#include "nesl2c/AST/NotOr.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

NotOr::NotOr(Node* a, Node* b) : LogicOperation(a, b)
{
}

void NotOr::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
