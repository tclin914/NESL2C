//===- IfElse.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/IfElse.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

IfElse::IfElse(Node* pA, Node* pB, Node* pC) : TernaryNode(pA, pB, pC)
{
}

bool IfElse::Initialize()
{
  return TernaryNode::Initialize();
}

bool IfElse::SemanticCheck()
{
  return TernaryNode::SemanticCheck();
}

void IfElse::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
