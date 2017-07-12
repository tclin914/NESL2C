#ifndef NESL2C_SEQUENCE_H
#define NESL2C_SEQUENCE_H

#include "BinaryNode.h"

namespace nesl2c {

class Sequence : public BinaryNode
{

public:
  Sequence(Node*, Node*);
};

} // namespace of nesl2c

#endif
