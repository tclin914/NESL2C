//===- TypeNode.cpp ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/TypeNode.h"
#include "nesl2c/Visitor/Visitor.h"

using namespace nesl2c;

TypeNode::TypeNode(NESLType pNESLType) : NullaryNode()
{
  f_NESLType = pNESLType;
}

bool TypeNode::Initialize()
{
  return true;
}

void TypeNode::Accept(Visitor* pVisitor)
{
  pVisitor->Visit(this);
}
