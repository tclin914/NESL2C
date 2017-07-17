//===- RelationOpFactory.h ----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_RELATION_OP_FACTORY_H
#define NESL2C_AST_RELATION_OP_FACTORY_H

#include "nesl2c/AST/OpCode.h"
#include "nesl2c/AST/Node.h"
#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class RelationalOpFactory
{

public:
  static RelationalOperation* CreateRelationalOpNode(OpCode, Node*, Node*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_RELATION_OP_FACTORY_H
