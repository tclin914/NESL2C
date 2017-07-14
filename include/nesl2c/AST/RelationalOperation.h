#ifndef NESL2C_RELATIONAL_OPERATION_H
#define NESL2C_RELATIONAL_OPERATION_H

#include "BinaryNode.h"

namespace nesl2c {

class RelationalOperation : public BinaryNode
{

public:
  RelationalOperation(Node*, Node*);

};

} // namespace of nesl2c

#endif
