#ifndef NESL2C_AST_NULLARY_NODE_H
#define NESL2C_AST_NULLARY_NODE_H

#include "nesl2c/AST/Node.h"

namespace nesl2c {

class NullaryNode : public Node
{

public:
  NullaryNode();
  virtual ~NullaryNode() {}

  virtual bool SemanticCheck();
}; 

} // end namespace nesl2c

#endif // NESL2C_AST_NULLARY_NODE_H
