#include <stdlib.h>

#include "ConstantFloat.h"
#include "Visitor.h"

using namespace nesl2c;

ConstantFloat::ConstantFloat(string num) : NullaryNode()
{
  this->num = atof(num.c_str());
}

bool ConstantFloat::Initialize()
{
  type = FLOAT_T;
  return true;
}

void ConstantFloat::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}
