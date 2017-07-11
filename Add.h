#ifndef NESL2C_ADD_H
#define NESL2C_ADD_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class Add : public ArithmeticOperation
{

public:
  Add(Node*, Node*);

};

} // namespace of nesl2c

#endif
