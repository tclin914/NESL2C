//===- XOr.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/XOr.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

XOr::XOr(Node* pA, Node* pB) : LogicOperation(pA, pB)
{
}

void XOr::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
