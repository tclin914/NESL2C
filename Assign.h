#ifndef NESL2C_ASSIGN_H
#define NESL2C_ASSIGN_H

#include "BinaryNode.h"

namespace nesl2c {

class Assign : public BinaryNode
{

public:
  Assign(Node*, Node*);
};

} // namespace of nesl2c

#endif
