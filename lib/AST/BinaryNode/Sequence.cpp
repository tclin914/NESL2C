//===- Sequence.cpp -----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Sequence.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Sequence::Sequence(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool Sequence::Initialize()
{
  return BinaryNode::Initialize();
}

bool Sequence::SemanticCheck()
{
  return BinaryNode::SemanticCheck();
}

void Sequence::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
