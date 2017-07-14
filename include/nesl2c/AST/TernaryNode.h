#ifndef NESL2C_TERNARY_NODE_H
#define NESL2C_TERNARY_NODE_H

#include "Node.h"

namespace nesl2c {

class TernaryNode : public Node
{

public:
  TernaryNode(Node*, Node*, Node*);

};

} // namespace of nesl2c

#endif
