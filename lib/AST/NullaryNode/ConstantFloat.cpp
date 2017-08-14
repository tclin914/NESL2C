//===- ConstantFloat.cpp ------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include <stdlib.h>

#include "nesl2c/AST/ConstantFloat.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ConstantFloat::ConstantFloat(string pValue) : NullaryNode()
{
  this->m_Value = atof(pValue.c_str());
}

void ConstantFloat::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}

float ConstantFloat::GetFPValue()
{
  return m_Value;
}
