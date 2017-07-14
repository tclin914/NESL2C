#ifndef NESL2C_AST_APPLY_BODY_H
#define NESL2C_AST_APPLY_BODY_H

#include "BinaryNode.h"

namespace nesl2c {

class ApplyBody : public BinaryNode
{

public:
  ApplyBody(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_APPLY_BODY_H
