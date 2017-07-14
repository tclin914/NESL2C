#ifndef NESL2C_RARROW_H
#define NESL2C_RARROW_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class RARROW : public ArithmeticOperation
{

public:
  RARROW(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
