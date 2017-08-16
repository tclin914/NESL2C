//===- Visitor.cpp ------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/Visitor/Visitor.h"
#include "nesl2c/AST/Node.h"

using namespace nesl2c;

void Visitor::VisitChildren(Node* pNode, int pNum)
{
  for (int i = 0; i < pNum; ++i) {
    if (NULL != pNode->GetChild(i))
      pNode->GetChild(i)->Accept(this); 
  }
}

void Visitor::VisitChild(Node* pNode, int pIndex) 
{
  pNode->GetChild(pIndex)->Accept(this);
}

void Visitor::VisitSelf(Node* pNode)
{
  pNode->Accept(this);
}
