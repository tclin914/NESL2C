//===- And.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/And.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

And::And(Node* pA, Node* pB) : LogicOperation(pA, pB)
{
}

void And::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
