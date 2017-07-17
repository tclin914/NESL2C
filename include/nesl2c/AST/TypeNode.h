//===- TypeNode.h -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_TYPE_NODE_H
#define NESL2C_AST_TYPE_NODE_H

#include "nesl2c/AST/NullaryNode.h"

namespace nesl2c {

class TypeNode : public NullaryNode
{

public:
  TypeNode(NESLType);
  
  bool Initialize();
  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_TYPE_NODE_H
