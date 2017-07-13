#ifndef NESL2C_CONSTANT_STRING_H
#define NESL2C_CONSTANT_STRING_H

#include <string>

#include "NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantString : public NullaryNode
{

public:
  ConstantString(string);
  
  bool Initialize();
  void Accept(Visitor*);

private:
  string str;

};

} // namespace of nesl2c

#endif
