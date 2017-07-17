//===- ConstantString.h -------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_CONSTANT_STRING_H
#define NESL2C_AST_CONSTANT_STRING_H

#include <string>

#include "nesl2c/AST/NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantString : public NullaryNode
{

public:
  ConstantString(string);
  
  bool Initialize();
  void Accept(Visitor*);

private:
  string m_Value;

};

} // end namespace nesl2c

#endif // NESL2C_AST_CONSTANT_STRING_H
