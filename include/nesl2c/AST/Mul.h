#ifndef NESL2C_AST_MUL_H
#define NESL2C_AST_MUL_H

#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class Mul : public ArithmeticOperation
{

public:
  Mul(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_MUL_H
