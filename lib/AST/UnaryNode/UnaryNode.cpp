//===- UnaryNode.cpp ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/UnaryNode.h"

using namespace nesl2c;

UnaryNode::UnaryNode(Node* pA) : Node()
{
  children.push_back(a);
}

bool UnaryNode::Initialize()
{
  if (children[0] != NULL)
    return children[0]->Initialize();
  return true;
}

bool UnaryNode::SemanticCheck()
{
  if (children[0] != NULL) 
    return children[0]->SemanticCheck();
  return true;
}
