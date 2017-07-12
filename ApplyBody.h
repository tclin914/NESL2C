#ifndef NESL2C_APPLY_BODY_H
#define NESL2C_APPLY_BODY_H

#include "BinaryNode.h"

namespace nesl2c {

class ApplyBody : public BinaryNode
{

public:
  ApplyBody(Node*, Node*);
};

} // namespace of nesl2c

#endif
