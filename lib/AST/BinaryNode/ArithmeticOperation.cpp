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
    f_NESLType = f_Children[0]->GetNESLType();
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

  if (f_Children[0]->GetNESLType() != f_Children[1]->GetNESLType() ||
        (f_Children[0]->GetNESLType() != INTEGER_T && 
        f_Children[0]->GetNESLType() != FLOAT_T)) {
    return false;
  }

  return true;
}
