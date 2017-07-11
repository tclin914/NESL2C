
#include "ArithmeticOpFactory.h"
#include "Add.h"
#include "Subtract.h"
#include "PP.h"

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
    default:
      return NULL;
      break; 
  }
}
