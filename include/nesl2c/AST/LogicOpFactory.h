#ifndef NESL2C_AST_LOGIC_OP_FACTORY_H
#define NESL2C_AST_LOGIC_OP_FACTORY_H

#include "nesl2c/AST/OpCode.h"
#include "nesl2c/AST/Node.h"
#include "nesl2c/AST/LogicOperation.h"

namespace nesl2c {

class LogicOpFactory
{

public:
  static LogicOperation* CreateLogicOpNode(OpCode, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_LOGIC_OP_FACTORY_H
