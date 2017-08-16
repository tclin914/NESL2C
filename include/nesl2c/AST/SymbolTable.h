//===- SymbolTable.h ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_SYMBOL_TABLE_H
#define NESL2C_SYMBOL_TABLE_H

#include <string>
#include <vector>

using namespace std;

namespace nesl2c {

class Symbol;

class SymbolTable
{

public:
  SymbolTable();
  
  unsigned int size() const { return m_Symbols.size(); }
  bool empty() const { return m_Symbols.empty(); }

  void addSymbol(Symbol*);
  Symbol* getSymbol(string pID);

  void dump();

private:
  typedef vector<Symbol*> Symbols;

private:
  Symbols m_Symbols;

};

} // namespace of nesl2c

#endif // NESL2C_SYMBOL_TABLE_H
