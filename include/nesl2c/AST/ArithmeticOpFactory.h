#ifndef NESL2C_ARITHMETIC_OP_FACTORY_H
#define NESL2C_ARITHMETIC_OP_FACTORY_H

#include "OpCode.h"
#include "Node.h"
#include "ArithmeticOperation.h"

namespace nesl2c {

class ArithmeticOpFactory 
{

public:
  static ArithmeticOperation* CreateArithmeticOpNode(OpCode, Node*, Node*);

};

} // namespace of nesl2c

#endif
