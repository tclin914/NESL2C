#ifndef NESL2C_LESS_EQUAL_H
#define NESL2C_LESS_EQUAL_H

#include "RelationalOperation.h"

namespace nesl2c {

class LessEqual : public RelationalOperation
{

public:
  LessEqual(Node*, Node*);

};

} // namespace of nesl2c

#endif
