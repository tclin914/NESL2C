#ifndef NESL2C_AST_RELATIONAL_OPERATION_H
#define NESL2C_AST_RELATIONAL_OPERATION_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class RelationalOperation : public BinaryNode
{

public:
  RelationalOperation(Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_RELATIONAL_OPERATION_H
