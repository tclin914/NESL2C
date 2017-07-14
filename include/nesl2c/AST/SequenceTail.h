#ifndef NESL2C_AST_SEQUENCE_TAIL_H
#define NESL2C_AST_SEQUENCE_TAIL_H

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

} // end namespace nesl2c

#endif // NESL2C_AST_SEQUENCE_TAIL_H
