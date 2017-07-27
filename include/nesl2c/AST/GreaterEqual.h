//===- GreaterEqual.h ---------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_GREATER_EQUAL_H
#define NESL2C_AST_GREATER_EQUAL_H

#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class GreaterEqual : public RelationalOperation
{

public:
  GreaterEqual(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_GREATER_EQUAL_H
