
#include "TypeNode.h"
#include "Visitor.h"

using namespace nesl2c;

TypeNode::TypeNode(Type type) : NullaryNode()
{
  this->type = type;
}

bool TypeNode::Initialize()
{
  return true;
}

void TypeNode::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
