
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
  if (children[0] == NULL || children[1] == NULL) {
    return false;
  }

  if (children[0]->GetType() != children[1]->GetType() ||
        (children[0]->GetType() != INTEGER_T && 
        children[0]->GetType() != FLOAT_T)) {
    return false;
  }

  return true;
}
