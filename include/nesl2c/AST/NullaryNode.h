//===- NullaryNode.h ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_NULLARY_NODE_H
#define NESL2C_AST_NULLARY_NODE_H

#include "nesl2c/AST/Node.h"

namespace nesl2c {

class NullaryNode : public Node
{

public:
  NullaryNode();
  virtual ~NullaryNode() {}

  virtual bool SemanticCheck();

}; 

} // namespace of nesl2c

#endif // NESL2C_AST_NULLARY_NODE_H
