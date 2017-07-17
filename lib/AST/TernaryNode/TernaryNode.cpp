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
  children.push_back(pA);
  children.push_back(pB);
  children.push_back(pC);
  m_NESLType = UNDEFINED;
}
