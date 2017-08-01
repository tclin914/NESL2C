//===- Upt.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Upt.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Upt::Upt(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool Upt::Initialize()
{
  return BinaryNode::Initialize();
}

bool Upt::SemanticCheck()
{
  return BinaryNode::SemanticCheck();
}

void Upt::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
