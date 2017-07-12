#ifndef NESL2C_EMPTY_SEQUENCE_H
#define NESL2C_EMPTY_SEQUENCE_H

#include "UnaryNode.h"

namespace nesl2c {

class EmptySequence : public UnaryNode
{

public:
  EmptySequence(Node*);
};

} // namespace of nesl2c

#endif
