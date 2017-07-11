#ifndef NESL2C_CONSTANT_FLOAT_H
#define NESL2C_CONSTANT_FLOAT_H

#include <string>

#include "NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantFloat : public NullaryNode
{

public:
  ConstantFloat(string);

private:
  float num;

};

} // namespace of nesl2c

#endif
