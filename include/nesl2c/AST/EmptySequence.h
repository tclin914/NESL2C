#ifndef NESL2C_AST_EMPTY_SEQUENCE_H
#define NESL2C_AST_EMPTY_SEQUENCE_H

#include "UnaryNode.h"

namespace nesl2c {

class EmptySequence : public UnaryNode
{

public:
  EmptySequence(Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_EMPTY_SEQUENCE_H
