
#include "ConstantString.h"

using namespace nesl2c;

ConstantString::ConstantString(string str) : NullaryNode()
{
  this->str = str;
}
