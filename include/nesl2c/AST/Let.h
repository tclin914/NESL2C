#ifndef NESL2C_AST_LET_H
#define NESL2C_AST_LET_H

#include "BinaryNode.h"

namespace nesl2c {

class Let : public BinaryNode
{

public:
  Let(Node*, Node*);
};

} // end namespace nesl2c

#endif // NESL2C_AST_LET_H
