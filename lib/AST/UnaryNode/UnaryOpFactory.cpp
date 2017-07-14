
#include "nesl2c/AST/UnaryOpFactory.h"
#include "nesl2c/AST/Sharp.h"
#include "nesl2c/AST/At.h"
#include "nesl2c/AST/Uminus.h"

using namespace nesl2c;

UnaryOperation* UnaryOpFactory::CreateUnaryOpNode(OpCode opcode, Node* a)
{
  switch (opcode) {
    case SHARP_OP:
      return new Sharp(a);
      break;
    case AT_OP:
      return new At(a);
      break;
    case UMINUS_OP: 
      return new Uminus(a);
      break;
    default:
      // TODO:
      return NULL;
      break;
  }
}