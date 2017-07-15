#ifndef NESL2C_AST_RELATION_OP_FACTORY_H
#define NESL2C_AST_RELATION_OP_FACTORY_H

#include "nesl2c/AST/OpCode.h"
#include "nesl2c/AST/Node.h"
#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class RelationalOpFactory
{

public:
  static RelationalOperation* CreateRelationalOpNode(OpCode, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_RELATION_OP_FACTORY_H
