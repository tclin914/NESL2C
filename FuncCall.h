#ifndef NESL2C_FUNC_CALL_H
#define NESL2C_FUNC_CALL_H

#include "BinaryNode.h"

namespace nesl2c {

class FuncCall : public BinaryNode
{

public:
  FuncCall(Node*, Node*);
};

} // namespace of nesl2c

#endif
