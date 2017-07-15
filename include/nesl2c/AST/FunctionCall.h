#ifndef NESL2C_AST_FUNCTION_CALL_H
#define NESL2C_AST_FUNCTION_CALL_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class FunctionCall : public BinaryNode
{

public:
  FunctionCall(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_FUNCTION_CALL_H
