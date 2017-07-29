//===- ApplyBody.cpp ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ApplyBody.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ApplyBody::ApplyBody(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool ApplyBody::Initialize()
{
  return BinaryNode::Initialize();
}

bool ApplyBody::SemanticCheck()
{
  return BinaryNode::SemanticCheck();
}

void ApplyBody::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
