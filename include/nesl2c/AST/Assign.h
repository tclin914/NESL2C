//===- Assign.h ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_ASSIGN_H
#define NESL2C_AST_ASSIGN_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class Assign : public BinaryNode
{

public:
  Assign(Node*, Node*);
};

} // namespace of nesl2c

#endif // NESL2C_AST_ASSIGN_H
