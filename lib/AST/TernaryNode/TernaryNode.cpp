//===- TernaryNode.cpp --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/TernaryNode.h"

using namespace nesl2c;

TernaryNode::TernaryNode(Node* pA, Node* pB, Node* pC) : Node()
{
  f_Children.push_back(pA);
  f_Children.push_back(pB);
  f_Children.push_back(pC);
  f_NESLType = UNDEFINED;
}

bool TernaryNode::Initialize()
{
  bool result = true;
  if (f_Children[0] != NULL) {
    result &= f_Children[0]->Initialize();   
  }
  if (f_Children[1] != NULL) {
    result &= f_Children[1]->Initialize();
  }
  if (f_Children[2] != NULL) {
    result &= f_Children[2]->Initialize();
  }
  return result;
}

bool TernaryNode::SemanticCheck()
{
  bool result = true;
  if (f_Children[0] != NULL) {
    result &= f_Children[0]->SemanticCheck();
  }
  if (f_Children[1] != NULL) {
    result &= f_Children[1]->SemanticCheck();
  }
  if (f_Children[2] != NULL) {
    result &= f_Children[2]->SemanticCheck();
  }
  return result;
}
