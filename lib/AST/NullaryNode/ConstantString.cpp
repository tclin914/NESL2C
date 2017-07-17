//===- ConstantString.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ConstantString.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ConstantString::ConstantString(string pValue) : NullaryNode()
{
  this->m_Value = pValue;
}

bool ConstantString::Initialize()
{
  m_NESLType = STRING_T;
  return true;
}

void ConstantString::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
