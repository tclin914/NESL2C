//===- Assign.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Assign.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Assign::Assign(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

void Assign::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
