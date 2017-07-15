#ifndef NESL2C_AST_GREATER_EQUAL_H
#define NESL2C_AST_GREATER_EQUAL_H

#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class GreaterEqual : public RelationalOperation
{

public:
  GreaterEqual(Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_GREATER_EQUAL_H
