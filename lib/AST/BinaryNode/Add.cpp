//===- Add.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Add.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Add::Add(Node* pA, Node* pB) : ArithmeticOperation(pA, pB)
{
}

void Add::Accept(Visitor* pVisitor)
{ 
  pVisitor->Visit(this);
}
