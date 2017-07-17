//===- Subtrace.h -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_SUBTRACT_H
#define NESL2C_AST_SUBTRACT_H

#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class Subtract : public ArithmeticOperation
{

public:
  Subtract(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_SUBTRACT_H
