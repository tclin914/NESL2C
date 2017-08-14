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
