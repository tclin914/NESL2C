#ifndef NESL2C_UNARY_OP_FACTORY_H
#define NESL2C_UNARY_OP_FACTORY_H

#include "OpCode.h"
#include "Node.h"
#include "UnaryOperation.h"

namespace nesl2c {

class UnaryOpFactory
{

public:
  static UnaryOperation* CreateUnaryOpNode(OpCode, Node*);

};

} // namespace of nesl2c

#endif
