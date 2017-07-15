
#include "nesl2c/AST/TypeNode.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

TypeNode::TypeNode(NESLType type) : NullaryNode()
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
