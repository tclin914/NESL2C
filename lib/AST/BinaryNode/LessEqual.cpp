//===- LessEqual.cpp ----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/LessEqual.h"

using namespace nesl2c;

LessEqual::LessEqual(Node* pA, Node* pB) : RelationalOperation(pA, pB)
{
}
