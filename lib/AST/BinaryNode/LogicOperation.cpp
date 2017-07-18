//===- LogicOperation.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/LogicOperation.h"

using namespace nesl2c;

LogicOperation::LogicOperation(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool LogicOperation::Initialize()
{
  if (!BinaryNode::Initialize())
    return false;

  bool result = locallySemanticCheck();
  if (result) 
    f_NESLType = f_Children[0]->GetType();
  return result;
}

bool LogicOperation::SemanticCheck()
{
  return BinaryNode::SemanticCheck() & locallySemanticCheck();
}

bool LogicOperation::locallySemanticCheck()
{
  // TODO:
  return true;
}
