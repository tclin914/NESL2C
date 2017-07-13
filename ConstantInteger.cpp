#include <stdlib.h>

#include "ConstantInteger.h"
#include "Visitor.h"

using namespace nesl2c;

ConstantInteger::ConstantInteger(string num) : NullaryNode()
{
  this->num = atoi(num.c_str());
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
