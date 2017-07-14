
#include "nesl2c/AST/BinaryNode.h"

using namespace nesl2c;

BinaryNode::BinaryNode(Node* a, Node* b) : Node()
{
  children.push_back(a);
  children.push_back(b);
  type = UNDEFINED;
}

bool BinaryNode::Initialize()
{
  bool result = true;
  if (children[0] != NULL) {
    result &= children[0]->Initialize();
  }
  if (children[1] != NULL) {
    result &= children[1]->Initialize();
  }
  return result;
}

bool BinaryNode::SemanticCheck()
{
  bool result = true;
  if (children[0] != NULL) {
    result &= children[0]->SemanticCheck();
  }
  if (children[1] != NULL) {
    result &= children[1]->SemanticCheck();
  }
  return result;
}
