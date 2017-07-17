//===- IfElse.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/IfElse.h"

using namespace nesl2c;

IfElse::IfElse(Node* pA, Node* pB, Node* pC) : TernaryNode(pA, pB, pC)
{
}
