//===- Upt.h ------------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_UPT_H
#define NESL2C_AST_UPT_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class Upt : public BinaryNode
{

public:
  Upt(Node*, Node*);

  void Accept(Visitor*);
  
};

} // namespace of nesl2c

#endif // NESL2C_AST_UPT_H
