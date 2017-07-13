#ifndef NESL2C_ARITHMETIC_OPERATION_H
#define NESL2C_ARITHMETIC_OPERATION_H

#include "BinaryNode.h"

namespace nesl2c {

class ArithmeticOperation : public BinaryNode
{

public:
  ArithmeticOperation(Node*, Node*);
  virtual ~ArithmeticOperation() {}

  bool Initialize(); 
  bool SemanticCheck();

private:
  bool locallySemanticCheck();

};

} // namespace of nesl2c

#endif
