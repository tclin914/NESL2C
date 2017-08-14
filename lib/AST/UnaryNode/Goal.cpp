//===- Goal.cpp ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Goal.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Goal::Goal(Node *pA) : UnaryNode(pA)
{
}

void Goal::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
