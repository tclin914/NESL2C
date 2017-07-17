//===- Add.h ------------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_ADD_H
#define NESL2C_AST_ADD_H

#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class Add : public ArithmeticOperation
{

public:
  Add(Node*, Node*);
  
  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_ADD_H
