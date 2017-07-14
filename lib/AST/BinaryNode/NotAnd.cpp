
#include "nesl2c/AST/NotAnd.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

NotAnd::NotAnd(Node* a, Node* b) : LogicOperation(a, b)
{
}

void NotAnd::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
