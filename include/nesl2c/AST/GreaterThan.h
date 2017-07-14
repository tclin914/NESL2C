#ifndef NESL2C_AST_GREATER_THAN_H
#define NESL2C_AST_GREATER_THAN_H

#include "RelationalOperation.h"

namespace nesl2c {

class GreaterThan : public RelationalOperation
{

public:
  GreaterThan(Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_GREATER_THAN_H
