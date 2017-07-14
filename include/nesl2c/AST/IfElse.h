#ifndef NESL2C_AST_IF_ELSE_H
#define NESL2C_AST_IF_ELSE_H

#include "TernaryNode.h"

namespace nesl2c {

class IfElse : public TernaryNode
{

public:
  IfElse(Node*, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_IF_ELSE_H
