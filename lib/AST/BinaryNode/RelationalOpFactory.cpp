//===- RelationalOpFactory.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/RelationalOpFactory.h"
#include "nesl2c/AST/Equal.h"
#include "nesl2c/AST/NotEqual.h"
#include "nesl2c/AST/LessThan.h"
#include "nesl2c/AST/GreaterThan.h"
#include "nesl2c/AST/LessEqual.h"
#include "nesl2c/AST/GreaterEqual.h"

using namespace nesl2c;

RelationalOperation* RelationalOpFactory::CreateRelationalOpNode(OpCode pOpcode, Node* pA, Node* pB)
{
  switch (pOpcode) {
    case EQ_OP:
      return new Equal(pA, pB);
      break;
    case NE_OP:
      return new NotEqual(pA, pB);
      break;
    case LT_OP:
      return new LessThan(pA, pB);
      break;
    case GT_OP:
      return new GreaterThan(pA, pB);
      break;
    case LE_OP:
      return new LessEqual(pA, pB);
      break;
    case GE_OP:
      return new GreaterEqual(pA, pB);
      break;
   default:
      // TODO:
      return NULL;
      break;
  }
}
