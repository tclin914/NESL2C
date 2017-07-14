#ifndef NESL2C_IF_ELSE_H
#define NESL2C_IF_ELSE_H

#include "TernaryNode.h"

namespace nesl2c {

class IfElse : public TernaryNode
{

public:
  IfElse(Node*, Node*, Node*);

};

} // namespace of nesl2c

#endif
