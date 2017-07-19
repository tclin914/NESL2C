//===- TopLevels.cpp ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/TopLevels.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

TopLevels::TopLevels(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

void TopLevels::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
