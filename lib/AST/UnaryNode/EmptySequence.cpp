
#include "EmptySequence.h"
#include "Visitor.h"

using namespace nesl2c;

EmptySequence::EmptySequence(Node* a) : UnaryNode(a)
{
}

void EmptySequence::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
