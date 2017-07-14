#ifndef NESL2C_AST_LESS_EQUAL_H
#define NESL2C_AST_LESS_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class LessEqual : public RelationalOperation
{

public:
  LessEqual(Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_LESS_EQUAL_H
