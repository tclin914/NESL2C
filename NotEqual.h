#ifndef NESL2C_NOT_EQUAL_H
#define NESL2C_NOT_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class NotEqual : public RelationalOperation
{
  
public:
 NotEqual(Node*, Node*); 

};

} // namespace of nesl2c

#endif
