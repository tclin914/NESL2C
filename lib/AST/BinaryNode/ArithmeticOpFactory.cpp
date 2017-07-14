
#include "nesl2c/AST/ArithmeticOpFactory.h"
#include "nesl2c/AST/Add.h"
#include "nesl2c/AST/Subtract.h"
#include "nesl2c/AST/PP.h"
#include "nesl2c/AST/LARROW.h"
#include "nesl2c/AST/Mul.h"
#include "nesl2c/AST/Div.h"
#include "nesl2c/AST/RARROW.h"

using namespace nesl2c;

ArithmeticOperation* ArithmeticOpFactory::CreateArithmeticOpNode(OpCode opcode, Node* a, Node* b)
{
  switch (opcode) {
    case ADD_OP:
      return new Add(a, b);      
      break;
    case SUBTRACT_OP:
      return new Subtract(a, b);
      break;
    case PP_OP:
      return new PP(a, b);
      break;
    case LARROW_OP:
      return new LARROW(a, b);
      break;
    case MUL_OP:
      return new Mul(a, b);
      break;
    case DIV_OP:
      return new Div(a, b);
      break;
    case RARROW_OP:
      return new RARROW(a, b);
      break;
    default:
      // TODO:
      return NULL;
      break; 
  }
}
