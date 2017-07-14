#ifndef NESL2C_AST_CONSTANT_FLOAT_H
#define NESL2C_AST_CONSTANT_FLOAT_H

#include <string>

#include "NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantFloat : public NullaryNode
{

public:
  ConstantFloat(string);
  
  bool Initialize();
  void Accept(Visitor*);

private:
  float num;

};

} // end namespace nesl2c

#endif // NESL2C_AST_CONSTANT_FLOAT_H
