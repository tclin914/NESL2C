//===- Tuple.cpp --------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Tuple.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Tuple::Tuple(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

void Tuple::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
