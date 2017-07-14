#ifndef NESL2C_MUL_H
#define NESL2C_MUL_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class Mul : public ArithmeticOperation
{

public:
  Mul(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
