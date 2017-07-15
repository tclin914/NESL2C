#ifndef NESL2C_AST_BINARY_NODE_H
#define NESL2C_AST_BINARY_NODE_H

#include "nesl2c/AST/Node.h"

namespace nesl2c {

class BinaryNode : public Node
{
 
public:
  BinaryNode(Node*, Node*);
  virtual ~BinaryNode() {}

  virtual bool Initialize();
  virtual bool SemanticCheck();
};

} // end namespace nesl2c

#endif // NESL2C_AST_BINARY_NODE_H
