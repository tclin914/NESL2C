//===- GreaterThan.h ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_GREATER_THAN_H
#define NESL2C_AST_GREATER_THAN_H

#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class GreaterThan : public RelationalOperation
{

public:
  GreaterThan(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_GREATER_THAN_H
