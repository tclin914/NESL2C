//===- Div.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Div.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Div::Div(Node* pA, Node* pB) : ArithmeticOperation(pA, pB)
{
}

void Div::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
