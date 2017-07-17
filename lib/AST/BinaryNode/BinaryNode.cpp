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
  children.push_back(pA);
  children.push_back(pB);
  m_NESLType = UNDEFINED;
}

bool BinaryNode::Initialize()
{
  bool result = true;
  if (children[0] != NULL) {
    result &= children[0]->Initialize();
  }
  if (children[1] != NULL) {
    result &= children[1]->Initialize();
  }
  return result;
}

bool BinaryNode::SemanticCheck()
{
  bool result = true;
  if (children[0] != NULL) {
    result &= children[0]->SemanticCheck();
  }
  if (children[1] != NULL) {
    result &= children[1]->SemanticCheck();
  }
  return result;
}
