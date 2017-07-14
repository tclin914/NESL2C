#ifndef NESL2C_CONSTANT_INTEGER_H
#define NESL2C_CONSTANT_INTEGER_H

#include <string>

#include "NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantInteger : public NullaryNode
{

public:
  ConstantInteger(string);
  
  bool Initialize();
  void Accept(Visitor*);

private:
  int num;

};

} // namespace of nesl2c

#endif
