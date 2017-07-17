//===- Node.h -----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_NODE_H
#define NESL2C_AST_NODE_H

#include <vector>

#include "nesl2c/AST/NESLType.h"

namespace nesl2c {

class Visitor;

class Node 
{

public:
  Node();
  virtual ~Node() {}

  virtual bool Initialize() = 0;
  virtual bool SemanticCheck() = 0;
  virtual void Accept(Visitor*) = 0;

  virtual NESLType GetType();

  Node* GetChild(int);

protected:
  NESLType m_NESLType;
  int m_LineNo;

protected:
  // virtual void ErrorReport(string, ...);
  std::vector<Node*> children;

};

} // namespace of nesl2c

#endif // NESL2C_AST_NODE_H
