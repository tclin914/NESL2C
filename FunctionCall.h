#ifndef NESL2C_FUNCTION_CALL_H
#define NESL2C_FUNCTION_CALL_H

#include "BinaryNode.h"

namespace nesl2c {

class FunctionCall : public BinaryNode
{

public:
  FunctionCall(Node*, Node*);
};

} // namespace of nesl2c

#endif
