//===- ApplyToEach.cpp --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ApplyToEach.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ApplyToEach::ApplyToEach(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

void ApplyToEach::Accept(Visitor* pVisitor)
{
  return pVisitor->Visit(this);
}
