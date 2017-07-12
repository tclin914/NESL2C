
#include "UnaryNode.h"

using namespace nesl2c;

UnaryNode::UnaryNode(Node* a) : Node()
{
  children.push_back(a);
}
