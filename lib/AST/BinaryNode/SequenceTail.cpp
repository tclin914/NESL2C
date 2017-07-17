//===- SequenceTail.cpp -------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/SequenceTail.h"
#include "nesl2c/AST/Visitor.h"

using namespace nesl2c;

SequenceTail::SequenceTail(Node* pA, Node* pB) : BinaryNode(pA, pB)
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

void SequenceTail::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}


