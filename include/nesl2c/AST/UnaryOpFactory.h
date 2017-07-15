#ifndef NESL2C_AST_UNARY_OP_FACTORY_H
#define NESL2C_AST_UNARY_OP_FACTORY_H

#include "nesl2c/AST/OpCode.h"
#include "nesl2c/AST/Node.h"
#include "nesl2c/AST/UnaryOperation.h"

namespace nesl2c {

class UnaryOpFactory
{

public:
  static UnaryOperation* CreateUnaryOpNode(OpCode, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_UNARY_OP_FACTORY_H
