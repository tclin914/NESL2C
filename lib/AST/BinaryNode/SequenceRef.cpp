//===- SequenceRef.cpp --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/SequenceRef.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

SequenceRef::SequenceRef(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool SequenceRef::Initialize()
{
  return BinaryNode::Initialize();
}

bool SequenceRef::SemanticCheck()
{
  return BinaryNode::SemanticCheck();
}

void SequenceRef::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}

