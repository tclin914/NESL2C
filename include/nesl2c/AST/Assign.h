#ifndef NESL2C_AST_ASSIGN_H
#define NESL2C_AST_ASSIGN_H

#include "BinaryNode.h"

namespace nesl2c {

class Assign : public BinaryNode
{

public:
  Assign(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_ASSIGN_H
