
#include "nesl2c/AST/ConstantString.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ConstantString::ConstantString(string str) : NullaryNode()
{
  this->str = str;
}

bool ConstantString::Initialize()
{
  type = STRING_T;
  return true;
}

void ConstantString::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
