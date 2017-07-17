//===- LogicOpFactory.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/LogicOpFactory.h"
#include "nesl2c/AST/And.h"
#include "nesl2c/AST/NotAnd.h"
#include "nesl2c/AST/Or.h"
#include "nesl2c/AST/NotOr.h"
#include "nesl2c/AST/XOr.h"

using namespace nesl2c;

LogicOperation* LogicOpFactory::CreateLogicOpNode(OpCode pOpcode, Node* pA, Node* pB)
{
  switch (pOpcode) {
    case AND_OP:
      return new And(pA, pB);
      break;
    case NAND_OP:
      return new NotAnd(pA, pB);
      break;
    case OR_OP:
      return new Or(pA, pB);
      break;
    case NOR_OP:
      return new NotOr(pA, pB);
      break;
    case XOR_OP: 
      return new XOr(pA, pB);
      break;
    default:
      // TODO:
      return NULL;
      break;
  }
}

