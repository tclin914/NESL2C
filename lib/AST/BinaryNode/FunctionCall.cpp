//===- FunctionCall.cpp ------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/FunctionCall.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

FunctionCall::FunctionCall(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}

bool FunctionCall::Initialize()
{
  return BinaryNode::Initialize();
}

bool FunctionCall::SemanticCheck()
{
  return BinaryNode::SemanticCheck(); 
}

void FunctionCall::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
