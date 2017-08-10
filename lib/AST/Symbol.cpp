//===- Symbol.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Symbol.h"

using namespace nesl2c;

Symbol::Symbol(string pID, NESLType pType) : m_ID(pID), m_NESLType(pType)
{
}

string Symbol::GetID() const
{
  return m_ID;
}

NESLType Symbol::GetNESLType() const
{
  return m_NESLType;
}

void Symbol::SetNESLType(NESLType pType)
{
  m_NESLType = pType;
}

void Symbol::SetValue(llvm::Value* pValue)
{
  m_Value = pValue;
}
