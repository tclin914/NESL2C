//===- Let.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Let.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Let::Let(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

void Let::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
