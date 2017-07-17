//===- Equal.cpp --------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Equal.h"

using namespace nesl2c;

Equal::Equal(Node* pA, Node* pB) : RelationalOperation(pA, pB)
{
}
