
#include "SequenceTail.h"
#include "Visitor.h"

using namespace nesl2c;

SequenceTail::SequenceTail(Node* a, Node* b) : BinaryNode(a, b)
{
}

bool SequenceTail::Initialize()
{
  return BinaryNode::Initialize();
}

bool SequenceTail::SemanticCheck()
{
  return BinaryNode::SemanticCheck();
}

void SequenceTail::Accept(Visitor* visitor)
{
  visitor->Visit(this);
}


