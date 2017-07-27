//===- RelationalOperation.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/RelationalOperation.h"

using namespace nesl2c;

RelationalOperation::RelationalOperation(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool RelationalOperation::Initialize()
{
  if (!BinaryNode::Initialize()) {
    return false;
  }

  f_NESLType = BOOL_T;
  return true;
}

bool RelationalOperation::SemanticCheck()
{
  return BinaryNode::SemanticCheck() & locallySemanticCheck();
}

bool RelationalOperation::locallySemanticCheck()
{
  // TODO:
  return true;
}
