//===- Symbol.h ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_SYMBOL_H
#define NESL2C_SYMBOL_H

#include <string>

#include "nesl2c/AST/NESLType.h"

using namespace std;

namespace llvm {

class Value;
}

namespace nesl2c {

class Symbol
{

public:
  Symbol(string, NESLType);
  
  string GetID() const;
  NESLType GetNESLType() const;
  void SetNESLType(NESLType);

  llvm::Value* GetValue() const;
  void SetValue(llvm::Value*);

private:
  string m_ID;
  NESLType m_NESLType; 
  llvm::Value* m_Value;

};

} // namespace of nesl2c

#endif // NESL2C_SYMBOL_H
