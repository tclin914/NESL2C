//===- UnaryOperation.h -------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_UNARY_OPERATION_H
#define NESL2C_AST_UNARY_OPERATION_H

#include "nesl2c/AST/UnaryNode.h"

namespace nesl2c {

class UnaryOperation : public UnaryNode
{

public:
  UnaryOperation(Node*);
  virtual ~UnaryOperation() {}
 
};

} // namespace of nesl2c

#endif // NESL2C_AST_UNARY_OPERATION_H
