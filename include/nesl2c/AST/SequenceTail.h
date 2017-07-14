#ifndef NESL2C_SEQUENCE_TAIL_H
#define NESL2C_SEQUENCE_TAIL_H

#include "UnaryNode.h"
#include "BinaryNode.h"

namespace nesl2c {

class SequenceTail : public BinaryNode
{
  
public:  
  SequenceTail(Node*, Node*);

  bool Initialize();
  bool SemanticCheck();
  void Accept(Visitor*);
};

} // namespace of nesl2c

#endif
