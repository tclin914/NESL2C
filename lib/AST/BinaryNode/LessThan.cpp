//===- LessThan.cpp -----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/LessThan.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

LessThan::LessThan(Node* pA, Node* pB) : RelationalOperation(pA, pB)
{
}

void LessThan::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
