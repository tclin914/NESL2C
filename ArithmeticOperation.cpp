
#include "ArithmeticOperation.h"

using namespace nesl2c;

ArithmeticOperation::ArithmeticOperation(Node* a, Node* b) : BinaryNode(a, b)
{
}

bool ArithmeticOperation::Initialize()
{
  if (!BinaryNode::Initialize()) 
    return false;
  
  bool result =  locallySemanticCheck();
  if (result)
    type = children[0]->GetType();
  return result;
}

bool ArithmeticOperation::SemanticCheck()
{
  return BinaryNode::SemanticCheck() & locallySemanticCheck();
}

bool ArithmeticOperation::locallySemanticCheck()
{
  // TODO:
  return true;
}
