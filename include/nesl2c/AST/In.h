#ifndef NESL2C_AST_IN_H
#define NESL2C_AST_IN_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {
  
class In : public BinaryNode
{

public:
  In(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_IN_H
