#ifndef NESL2C_BINARY_NODE_H
#define NESL2C_BINARY_NODE_H

#include "Node.h"

namespace nesl2c {

class BinaryNode : public Node
{
 
public:
  BinaryNode(Node*, Node*);
  virtual ~BinaryNode() {}

  virtual bool Initialize();
  virtual bool SemanticCheck();
};

} // namespace of nesl2c

#endif
