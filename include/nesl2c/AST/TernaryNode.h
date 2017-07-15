#ifndef NESL2C_AST_TERNARY_NODE_H
#define NESL2C_AST_TERNARY_NODE_H

#include "nesl2c/AST/Node.h"

namespace nesl2c {

class TernaryNode : public Node
{

public:
  TernaryNode(Node*, Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_TERNARY_NODE_H
