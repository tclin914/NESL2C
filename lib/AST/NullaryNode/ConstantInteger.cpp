#include <stdlib.h>

#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ConstantInteger::ConstantInteger(string value) : NullaryNode()
{
  this->value = atoi(value.c_str());
}

bool ConstantInteger::Initialize()
{
  type = INTEGER_T;
  return true;
}

void ConstantInteger::Accept(Visitor* visitor) 
{
  visitor->Visit(this);
}

int ConstantInteger::GetIntValue() 
{
  return value;
}
