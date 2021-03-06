//===- NotAnd.h ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_NOT_AND_H
#define NESL2C_AST_NOT_AND_H

#include "nesl2c/AST/LogicOperation.h"

namespace nesl2c {

class NotAnd : public LogicOperation
{

public:
  NotAnd(Node*, Node*);
  
  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_NOT_AND_H
