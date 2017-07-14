#ifndef NESL2C_AST_RELATION_OP_FACTORY_H
#define NESL2C_AST_RELATION_OP_FACTORY_H

#include "OpCode.h"
#include "Node.h"
#include "RelationalOperation.h"

namespace nesl2c {

class RelationalOpFactory
{

public:
  static RelationalOperation* CreateRelationalOpNode(OpCode, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_RELATION_OP_FACTORY_H
