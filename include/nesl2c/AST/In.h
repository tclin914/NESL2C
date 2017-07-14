#ifndef NESL2C_IN_H
#define NESL2C_IN_H

#include "BinaryNode.h"

namespace nesl2c {
  
class In : public BinaryNode
{

public:
  In(Node*, Node*);
};

} // namespace of nesl2c

#endif
