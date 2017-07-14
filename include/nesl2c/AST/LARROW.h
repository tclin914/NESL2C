#ifndef NESL2C_AST_LARROW_H
#define NESL2C_AST_LARROW_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class LARROW : public ArithmeticOperation
{

public:
  LARROW(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_LARROW_H
