
#include "nesl2c/AST/And.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

And::And(Node* a, Node* b) : LogicOperation(a, b)
{
}

void And::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
