#ifndef NESL2C_EQUAL_H
#define NESL2C_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class Equal : public RelationalOperation
{

public:
  Equal(Node*, Node*);

};

} // namespace of nesl2c

#endif
