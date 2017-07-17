//===- NotOr.cpp --------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/NotOr.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

NotOr::NotOr(Node* pA, Node* pB) : LogicOperation(pA, pB)
{
}

void NotOr::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
