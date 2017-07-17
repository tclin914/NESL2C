//===- ConstantInteger.h ------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_CONSTANT_INTEGER_H
#define NESL2C_AST_CONSTANT_INTEGER_H

#include <string>

#include "nesl2c/AST/NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantInteger : public NullaryNode
{

public:
  ConstantInteger(string);
  
  bool Initialize();
  void Accept(Visitor*);

  int GetIntValue();

private:
  int m_Value;

};

} // namespace of nesl2c

#endif // NESL2C_AST_CONSTANT_INTEGER_H
