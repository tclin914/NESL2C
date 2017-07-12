#ifndef NESL2C_DIV_H
#define NESL2C_DIV_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class Div : public ArithmeticOperation
{
 
public:
  Div(Node*, Node*);
};


} // namespace of nesl2c

#endif
