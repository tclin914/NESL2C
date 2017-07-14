#ifndef NESL2C_AST_NOT_EQUAL_H
#define NESL2C_AST_NOT_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class NotEqual : public RelationalOperation
{
  
public:
 NotEqual(Node*, Node*); 

};

} // end namespace nesl2c

#endif // NESL2C_AST_NOT_EQUAL_H
