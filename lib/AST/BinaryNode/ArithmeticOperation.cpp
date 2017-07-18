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
    f_NESLType = f_Children[0]->GetType();
  return result;
}

bool ArithmeticOperation::SemanticCheck()
{
  return BinaryNode::SemanticCheck() & locallySemanticCheck();
}

bool ArithmeticOperation::locallySemanticCheck()
{
  if (f_Children[0] == NULL || f_Children[1] == NULL) {
    return false;
  }

  if (f_Children[0]->GetType() != f_Children[1]->GetType() ||
        (f_Children[0]->GetType() != INTEGER_T && 
        f_Children[0]->GetType() != FLOAT_T)) {
    return false;
  }

  return true;
}
