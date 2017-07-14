#ifndef NESL2C_CONSTANT_BOOLEAN_H
#define NESL2C_CONSTANT_BOOLEAN_H

#include "NullaryNode.h"

namespace nesl2c {

class ConstantBoolean : public NullaryNode
{

public:
  ConstantBoolean(bool);
  
  bool Initialize();
  void Accept(Visitor*);

private:
  bool value;

};

} // namespace of nesl2c

#endif
