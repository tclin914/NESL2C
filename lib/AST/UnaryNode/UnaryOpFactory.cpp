//===- UnaryOpFactory.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/UnaryOpFactory.h"
#include "nesl2c/AST/Sharp.h"
#include "nesl2c/AST/At.h"
#include "nesl2c/AST/Uminus.h"

using namespace nesl2c;

UnaryOperation* UnaryOpFactory::CreateUnaryOpNode(OpCode pOpcode, Node* pA)
{
  switch (pOpcode) {
    case SHARP_OP:
      return new Sharp(pA);
      break;
    case AT_OP:
      return new At(pA);
      break;
    case UMINUS_OP: 
      return new Uminus(pA);
      break;
    default:
      // TODO:
      return NULL;
      break;
  }
}
