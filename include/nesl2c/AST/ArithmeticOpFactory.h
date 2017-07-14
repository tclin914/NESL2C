#ifndef NESL2C_AST_ARITHMETIC_OP_FACTORY_H
#define NESL2C_AST_ARITHMETIC_OP_FACTORY_H

#include "OpCode.h"
#include "Node.h"
#include "ArithmeticOperation.h"

namespace nesl2c {

class ArithmeticOpFactory 
{

public:
  static ArithmeticOperation* CreateArithmeticOpNode(OpCode, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_ARITHMETIC_OP_FACTORY_H
