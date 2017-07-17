//===- ArithmeticOperation.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ArithmeticOperation.h"

using namespace nesl2c;

ArithmeticOperation::ArithmeticOperation(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool ArithmeticOperation::Initialize()
{
  if (!BinaryNode::Initialize()) 
    return false;
  
  bool result =  locallySemanticCheck();
  if (result)
    m_NESLType = children[0]->GetType();
  return result;
}

bool ArithmeticOperation::SemanticCheck()
{
  return BinaryNode::SemanticCheck() & locallySemanticCheck();
}

bool ArithmeticOperation::locallySemanticCheck()
{
  if (children[0] == NULL || children[1] == NULL) {
    return false;
  }

  if (children[0]->GetType() != children[1]->GetType() ||
        (children[0]->GetType() != INTEGER_T && 
        children[0]->GetType() != FLOAT_T)) {
    return false;
  }

  return true;
}
