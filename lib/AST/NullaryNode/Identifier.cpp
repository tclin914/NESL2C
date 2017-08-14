//===- Identifier.cpp ---------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Identifier.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

Identifier::Identifier(string pValue) : NullaryNode()
{
  this->f_ID = pValue;
}

void Identifier::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
