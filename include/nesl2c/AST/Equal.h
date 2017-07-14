#ifndef NESL2C_AST_EQUAL_H
#define NESL2C_AST_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class Equal : public RelationalOperation
{

public:
  Equal(Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_EQUAL_H
