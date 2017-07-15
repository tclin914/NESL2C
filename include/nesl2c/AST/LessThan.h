#ifndef NESL2C_AST_LESS_THAN_H
#define NESL2C_AST_LESS_THAN_H

#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class LessThan : public RelationalOperation
{

public:
  LessThan(Node*, Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_LESS_THAN_H
