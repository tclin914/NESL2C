#ifndef NESL2C_LET_H
#define NESL2C_LET_H

#include "BinaryNode.h"

namespace nesl2c {

class Let : public BinaryNode
{

public:
  Let(Node*, Node*);
};

} // namespace of nesl2c

#endif
