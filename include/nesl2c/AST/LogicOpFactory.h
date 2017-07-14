#ifndef NESL2C_AST_LOGIC_OP_FACTORY_H
#define NESL2C_AST_LOGIC_OP_FACTORY_H

#include "OpCode.h"
#include "Node.h"
#include "LogicOperation.h"

namespace nesl2c {

class LogicOpFactory
{

public:
  static LogicOperation* CreateLogicOpNode(OpCode, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_LOGIC_OP_FACTORY_H
