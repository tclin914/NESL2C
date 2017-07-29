//===- Uminus.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Uminus.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Uminus::Uminus(Node *pA) : UnaryOperation(pA)
{
}

void Uminus::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
