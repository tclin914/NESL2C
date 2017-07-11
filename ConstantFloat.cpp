#include <stdlib.h>

#include "ConstantFloat.h"

using namespace nesl2c;

ConstantFloat::ConstantFloat(string num) : NullaryNode()
{
  this->num = atof(num.c_str());
}
