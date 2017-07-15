#ifndef NESL2C_AST_RARROW_H
#define NESL2C_AST_RARROW_H

#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class RARROW : public ArithmeticOperation
{

public:
  RARROW(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_RARROW_H
