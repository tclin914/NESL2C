//===- RARROW.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/RARROW.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

RARROW::RARROW(Node* pA, Node* pB) : ArithmeticOperation(pA, pB)
{
}

void RARROW::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
