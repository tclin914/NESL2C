
#include "RelationalOpFactory.h"
#include "Equal.h"
#include "NotEqual.h"
#include "LessThan.h"
#include "GreaterThan.h"
#include "LessEqual.h"
#include "GreaterEqual.h"

using namespace nesl2c;

RelationalOperation* RelationalOpFactory::CreateRelationalOpNode(OpCode opcode, Node* a, Node *b)
{
  switch (opcode) {
    case EQ_OP:
      return new Equal(a, b);
      break;
    case NE_OP:
      return new NotEqual(a, b);
      break;
    case LT_OP:
      return new LessThan(a, b);
      break;
    case GT_OP:
      return new GreaterThan(a, b);
      break;
    case LE_OP:
      return new LessEqual(a, b);
      break;
    case GE_OP:
      return new GreaterEqual(a, b);
      break;
   default:
      // TODO:
      return NULL;
      break;
  }
}
