//===- Assign.cpp -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nes2lc/AST/Assign.h"

using namespace nesl2c;

Assign::Assign(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}
