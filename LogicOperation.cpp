
#include "LogicOperation.h"

using namespace nesl2c;

LogicOperation::LogicOperation(Node* a, Node* b) : BinaryNode(a, b)
{
}

bool LogicOperation::Initialize()
{
  if (!BinaryNode::Initialize())
    return false;

  bool result = locallySemanticCheck();
  if (result) 
    type = children[0]->GetType();
  return result;
}

bool LogicOperation::SemanticCheck()
{
  return BinaryNode::SemanticCheck() & locallySemanticCheck();
}

bool LogicOperation::locallySemanticCheck()
{
  // TODO:
  return true;
}
