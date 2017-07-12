
#include "LogicOpFactory.h"
#include "And.h"
#include "NotAnd.h"
#include "Or.h"
#include "NotOr.h"
#include "XOr.h"

using namespace nesl2c;

LogicOperation* LogicOpFactory::CreateLogicOpNode(OpCode opcode, Node* a, Node* b)
{
  switch (opcode) {
    case AND_OP:
      return new And(a, b);
      break;
    case NAND_OP:
      return new NotAnd(a, b);
      break;
    case OR_OP:
      return new Or(a, b);
      break;
    case NOR_OP:
      return new NotOr(a, b);
      break;
    case XOR_OP: 
      return new XOr(a, b);
      break;
    default:
      // TODO:
      return NULL;
      break;
  }
}

