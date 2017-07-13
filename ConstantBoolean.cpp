
#include "ConstantBoolean.h"
#include "Visitor.h"

using namespace nesl2c;

ConstantBoolean::ConstantBoolean(bool value) : NullaryNode()
{
  this->value = value;
}

bool ConstantBoolean::Initialize()
{
  type = BOOL_T;
  return true;
}

void ConstantBoolean::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
