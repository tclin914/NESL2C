#ifndef NESL2C_SUBTRACT_H
#define NESL2C_SUBTRACT_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class Subtract : public ArithmeticOperation
{

public:
  Subtract(Node*, Node*);

};

} // namespace of nesl2c

#endif
