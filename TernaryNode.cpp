
#include "TernaryNode.h"

using namespace nesl2c;

TernaryNode::TernaryNode(Node* a, Node* b, Node* c) : Node()
{
  children.push_back(a);
  children.push_back(b);
  children.push_back(c);
  type = UNDEFINED;
}
