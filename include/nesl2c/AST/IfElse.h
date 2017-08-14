//===- IfElse.h ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_IF_ELSE_H
#define NESL2C_AST_IF_ELSE_H

#include "nesl2c/AST/TernaryNode.h"

namespace nesl2c {

class IfElse : public TernaryNode
{

public:
  IfElse(Node*, Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_IF_ELSE_H
