//===- Or.cpp -----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Or.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Or::Or(Node* pA, Node* pB) : LogicOperation(pA, pB)
{
}

void Or::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
