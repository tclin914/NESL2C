//===- ArithmeticOperation.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ArithmeticOperation.h"

using namespace nesl2c;

ArithmeticOperation::ArithmeticOperation(Node* pA, Node* pB) : BinaryNode(pA, pB)
{
}
