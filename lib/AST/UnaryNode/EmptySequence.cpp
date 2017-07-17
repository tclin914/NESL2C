//===- EmptySequence.cpp ------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/EmptySequence.h"
#include "nesl2c/AST/Visitor.h"

using namespace nesl2c;

EmptySequence::EmptySequence(Node* pA) : UnaryNode(pA)
{
}

void EmptySequence::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
