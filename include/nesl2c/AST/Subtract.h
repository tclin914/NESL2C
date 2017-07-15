#ifndef NESL2C_AST_SUBTRACT_H
#define NESL2C_AST_SUBTRACT_H

#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class Subtract : public ArithmeticOperation
{

public:
  Subtract(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_SUBTRACT_H
