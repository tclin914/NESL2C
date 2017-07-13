#ifndef NESL2C_UNARY_NODE_H
#define NESL2C_UNARY_NODE_H

#include "Node.h"

namespace nesl2c {

class UnaryNode : public Node
{

public:
  UnaryNode(Node*);
  virtual ~UnaryNode() {}

  virtual bool Initialize();
  virtual bool SemanticCheck();
};

} // namespace of nesl2c

#endif
