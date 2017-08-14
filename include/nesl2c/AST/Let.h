//===- Let.h ------------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_LET_H
#define NESL2C_AST_LET_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class Let : public BinaryNode
{

public:
  Let(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_LET_H
