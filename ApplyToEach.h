#ifndef NESL2C_APPLY_TO_EACH_H
#define NESL2C_APPLY_TO_EACH_H

#include "BinaryNode.h"

namespace nesl2c {

class ApplyToEach : public BinaryNode
{

public:
  ApplyToEach(Node*, Node*);
};

} // namespace of nesl2c

#endif
