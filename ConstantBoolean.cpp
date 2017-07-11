
#include "ConstantBoolean.h"

using namespace nesl2c;

ConstantBoolean::ConstantBoolean(bool value) : NullaryNode()
{
  this->value = value;
}
