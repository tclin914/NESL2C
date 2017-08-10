//===- SymbolTable.cpp --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/SymbolTable.h"
#include "nesl2c/AST/Symbol.h"

using namespace nesl2c;

SymbolTable::SymbolTable()
{
}

void SymbolTable::addSymbol(Symbol* pSymbol)
{
  m_Symbols.push_back(pSymbol);
}

Symbol* SymbolTable::getSymbol(string pString)
{
  vector<Symbol*>::iterator it;
  for (it = m_Symbols.begin(); it != m_Symbols.end(); ++it) {
    if ((*it)->GetID() == pString) {
      return *it; 
    }    
  }
  return nullptr;
}
