
#include "BinaryNode.h"

using namespace nesl2c;

BinaryNode::BinaryNode(Node* a, Node* b) : Node()
{
  children.push_back(a);
  children.push_back(b);
  type = UNDEFINED;
}
