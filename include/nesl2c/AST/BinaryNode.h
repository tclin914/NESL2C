//===- BinaryNode.h -----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_BINARY_NODE_H
#define NESL2C_AST_BINARY_NODE_H

#include "nesl2c/AST/Node.h"

namespace nesl2c {

class BinaryNode : public Node
{
 
public:
  BinaryNode(Node*, Node*);
  virtual ~BinaryNode() {}

  virtual bool Initialize();
  virtual bool SemanticCheck();

};

} // namespace of nesl2c

#endif // NESL2C_AST_BINARY_NODE_H
