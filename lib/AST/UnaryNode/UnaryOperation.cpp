//===- UnaryOperation.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/UnaryOperation.h"

using namespace nesl2c;

UnaryOperation::UnaryOperation(Node* pA) : UnaryNode(pA)
{
}

bool UnaryOperation::Initialize()
{
  if (!UnaryNode::Initialize())
    return false;

  return true;
}

bool UnaryOperation::SemanticCheck()
{
  return UnaryNode::SemanticCheck();
}
