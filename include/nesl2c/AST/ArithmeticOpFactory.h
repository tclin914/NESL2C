#ifndef NESL2C_AST_ARITHMETIC_OP_FACTORY_H
#define NESL2C_AST_ARITHMETIC_OP_FACTORY_H

#include "nesl2c/AST/OpCode.h"
#include "nesl2c/AST/Node.h"
#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class ArithmeticOpFactory 
{

public:
  static ArithmeticOperation* CreateArithmeticOpNode(OpCode, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_ARITHMETIC_OP_FACTORY_H
