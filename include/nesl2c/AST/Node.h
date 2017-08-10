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
#include <string>

#include "nesl2c/AST/NESLType.h"

using namespace std;

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

  NESLType GetNESLType();
  void SetNESLType(NESLType);
  Node* GetChild(int);
  int GetChildNum();
  string GetID() const;
  bool isLeafNode() const;

protected:
  typedef std::vector<Node*> Children;

protected:
  NESLType f_NESLType;
  int f_LineNo;
  Children f_Children;
  string f_ID;

protected:
  // virtual void ErrorReport(string, ...);

};

} // namespace of nesl2c

#endif // NESL2C_AST_NODE_H
