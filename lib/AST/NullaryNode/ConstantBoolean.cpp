//===- ConstantBoolean.cpp ----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ConstantBoolean.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

ConstantBoolean::ConstantBoolean(bool pValue) : NullaryNode()
{
  this->m_Value = pValue;
}

bool ConstantBoolean::Initialize()
{
  m_NESLType = BOOL_T;
  return true;
}

void ConstantBoolean::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}

bool ConstantBoolean::GetBoolValue() 
{
  return m_Value;
}
