
#include "nesl2c/AST/UnaryNode.h"

using namespace nesl2c;

UnaryNode::UnaryNode(Node* a) : Node()
{
  children.push_back(a);
}

bool UnaryNode::Initialize()
{
  if (children[0] != NULL)
    return children[0]->Initialize();
  return true;
}

bool UnaryNode::SemanticCheck()
{
  if (children[0] != NULL) 
    return children[0]->SemanticCheck();
  return true;
}
