//===- ArithmeticOpFactory.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/ArithmeticOpFactory.h"
#include "nesl2c/AST/Add.h"
#include "nesl2c/AST/Subtract.h"
#include "nesl2c/AST/PP.h"
#include "nesl2c/AST/LARROW.h"
#include "nesl2c/AST/Mul.h"
#include "nesl2c/AST/Div.h"
#include "nesl2c/AST/RARROW.h"

using namespace nesl2c;

ArithmeticOperation* ArithmeticOpFactory::CreateArithmeticOpNode(OpCode pOpcode, Node* pA, Node* pB)
{
  switch (pOpcode) {
    case ADD_OP:
      return new Add(pA, pB);      
      break;
    case SUBTRACT_OP:
      return new Subtract(pA, pB);
      break;
    case PP_OP:
      return new PP(pA, pB);
      break;
    case LARROW_OP:
      return new LARROW(pA, pB);
      break;
    case MUL_OP:
      return new Mul(pA, pB);
      break;
    case DIV_OP:
      return new Div(pA, pB);
      break;
    case RARROW_OP:
      return new RARROW(pA, pB);
      break;
    default:
      // TODO:
      return NULL;
      break; 
  }
}
