//===- RelationOperation.h ----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_RELATIONAL_OPERATION_H
#define NESL2C_AST_RELATIONAL_OPERATION_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class RelationalOperation : public BinaryNode
{

public:
  RelationalOperation(Node*, Node*);
  virtual ~RelationalOperation() {}

};

} // namespace of nesl2c

#endif // NESL2C_AST_RELATIONAL_OPERATION_H
