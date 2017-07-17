//===- Mul.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Mul.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Mul::Mul(Node* pA, Node* pB) : ArithmeticOperation(pA, pB)
{
}

void Mul::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
