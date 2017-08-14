//===- ConstantFloat.h --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_CONSTANT_FLOAT_H
#define NESL2C_AST_CONSTANT_FLOAT_H

#include <string>

#include "nesl2c/AST/NullaryNode.h"

using namespace std;

namespace nesl2c {

class ConstantFloat : public NullaryNode
{

public:
  ConstantFloat(string);
  
  void Accept(Visitor*);

  float GetFPValue();

private:
  float m_Value;

};

} // namespace of nesl2c

#endif // NESL2C_AST_CONSTANT_FLOAT_H
