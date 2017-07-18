//===- BinaryNode.cpp ---------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/BinaryNode.h"

using namespace nesl2c;

BinaryNode::BinaryNode(Node* pA, Node* pB) : Node()
{
  f_Children.push_back(pA);
  f_Children.push_back(pB);
  f_NESLType = UNDEFINED;
}

bool BinaryNode::Initialize()
{
  bool result = true;
  if (f_Children[0] != NULL) {
    result &= f_Children[0]->Initialize();
  }
  if (f_Children[1] != NULL) {
    result &= f_Children[1]->Initialize();
  }
  return result;
}

bool BinaryNode::SemanticCheck()
{
  bool result = true;
  if (f_Children[0] != NULL) {
    result &= f_Children[0]->SemanticCheck();
  }
  if (f_Children[1] != NULL) {
    result &= f_Children[1]->SemanticCheck();
  }
  return result;
}
