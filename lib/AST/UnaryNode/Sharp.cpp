//===- Sharp.cpp --------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Sharp.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Sharp::Sharp(Node* pA) : UnaryOperation(pA)
{
}

void Sharp::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
