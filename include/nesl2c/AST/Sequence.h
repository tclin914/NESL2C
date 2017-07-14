#ifndef NESL2C_AST_SEQUENCE_H
#define NESL2C_AST_SEQUENCE_H

#include "BinaryNode.h"

namespace nesl2c {

class Sequence : public BinaryNode
{

public:
  Sequence(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_SEQUENCE_H
