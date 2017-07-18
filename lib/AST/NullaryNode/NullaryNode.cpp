//===- NullaryNode.cpp --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/NullaryNode.h"

using namespace nesl2c;

NullaryNode::NullaryNode() : Node()
{
  f_NESLType = UNDEFINED;
}

bool NullaryNode::SemanticCheck()
{
  return true;
}
