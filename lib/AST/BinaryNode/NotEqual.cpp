//===- NotEqual.cpp -----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/NotEqual.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

NotEqual::NotEqual(Node* pA, Node* pB) : RelationalOperation(pA, pB)
{
}

void NotEqual::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
