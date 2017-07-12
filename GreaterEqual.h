#ifndef NESL2C_GREATER_EQUAL_H
#define NESL2C_GREATER_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class GreaterEqual : public RelationalOperation
{

public:
  GreaterEqual(Node*, Node*);

};

} // namespace of nesl2c

#endif
