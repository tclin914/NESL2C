//===- TernaryNode.h ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_TERNARY_NODE_H
#define NESL2C_AST_TERNARY_NODE_H

#include "nesl2c/AST/Node.h"

namespace nesl2c {

class TernaryNode : public Node
{

public:
  TernaryNode(Node*, Node*, Node*);
  virtual ~TernaryNode() {}

  virtual bool Initialize();
  virtual bool SemanticCheck();

};

} // namespace of nesl2c

#endif // NESL2C_AST_TERNARY_NODE_H
