//===- ConstantInteger.cpp ----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include <stdlib.h>

#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ConstantInteger::ConstantInteger(string pValue) : NullaryNode()
{
  this->m_Value = atoi(pValue.c_str());
}

void ConstantInteger::Accept(Visitor* pVisitor) 
{
  pVisitor->Visit(this);
}

int ConstantInteger::GetIntValue() 
{
  return m_Value;
}
