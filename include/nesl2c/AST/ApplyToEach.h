#ifndef NESL2C_AST_APPLY_TO_EACH_H
#define NESL2C_AST_APPLY_TO_EACH_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class ApplyToEach : public BinaryNode
{

public:
  ApplyToEach(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_APPLY_TO_EACH_H
