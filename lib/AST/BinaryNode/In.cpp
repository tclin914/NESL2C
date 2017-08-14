//===- In.cpp -----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/In.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

In::In(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

void In::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
