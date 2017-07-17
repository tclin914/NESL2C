#ifndef NESL2C_AST_CONSTANT_BOOLEAN_H
#define NESL2C_AST_CONSTANT_BOOLEAN_H

#include "nesl2c/AST/NullaryNode.h"

namespace nesl2c {

class ConstantBoolean : public NullaryNode
{

public:
  ConstantBoolean(bool);
  
  bool Initialize();
  void Accept(Visitor*);

  bool GetBoolValue();

private:
  bool value;

};

} // end namespace nesl2c

#endif // NESL2C_AST_CONSTANT_BOOLEAN_H
