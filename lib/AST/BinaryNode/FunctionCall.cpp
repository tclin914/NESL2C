//===- FunctionCall.cpp ------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/FunctionCall.h"

using namespace nesl2c;

FunctionCall::FunctionCall(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}
