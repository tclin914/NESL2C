#ifndef NESL2C_AST_UNARY_NODE_H
#define NESL2C_AST_UNARY_NODE_H

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

} // end namespace nesl2c

#endif // NESL2C_AST_UNARY_NODE_H
