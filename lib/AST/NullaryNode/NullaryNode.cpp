
#include "nesl2c/AST/NullaryNode.h"

using namespace nesl2c;

NullaryNode::NullaryNode() : Node()
{
  type = UNDEFINED;
}

bool NullaryNode::SemanticCheck()
{
  return true;
}
