#ifndef NESL2C_LOGIC_OPERATION_H
#define NESL2C_LOGIC_OPERATION_H

#include "BinaryNode.h"

namespace nesl2c {

class LogicOperation : public BinaryNode
{

public:
  LogicOperation(Node*, Node*);
  virtual ~LogicOperation() {}

  bool Initialize();
  bool SemanticCheck();

private:
  bool locallySemanticCheck();
                    
};

} // namespace of nesl2c

#endif
