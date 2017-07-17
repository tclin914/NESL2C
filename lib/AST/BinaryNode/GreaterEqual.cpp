//===- GreaterEqual.cpp -------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/GreaterEqual.h"

using namespace nesl2c;

GreaterEqual::GreaterEqual(Node* pA, Node* pB) : RelationalOperation(pA, pB)
{
}
