#include <stdlib.h>

#include "ConstantInteger.h"

using namespace nesl2c;

ConstantInteger::ConstantInteger(string num) : NullaryNode()
{
  this->num = atoi(num.c_str());
}
