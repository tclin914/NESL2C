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
  f_Children.push_back(pA);
}

bool UnaryNode::Initialize()
{
  if (f_Children[0] != NULL)
    return f_Children[0]->Initialize();
  return true;
}

bool UnaryNode::SemanticCheck()
{
  if (f_Children[0] != NULL) 
    return f_Children[0]->SemanticCheck();
  return true;
}
