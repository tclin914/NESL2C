
#include "Identifier.h"
#include "Visitor.h"

using namespace nesl2c;

Identifier::Identifier(string str) : NullaryNode()
{
  this->str = str;
}

bool Identifier::Initialize()
{
  type = UNDEFINED;
  return true;
}

void Identifier::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
