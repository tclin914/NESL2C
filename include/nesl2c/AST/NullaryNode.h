#ifndef NESL2C_NULLARY_NODE_H
#define NESL2C_NULLARY_NODE_H

#include "Node.h"

namespace nesl2c {

class NullaryNode : public Node
{

public:
  NullaryNode();
  virtual ~NullaryNode() {}

  virtual bool SemanticCheck();
}; 

} // namespace of nesl2c

#endif