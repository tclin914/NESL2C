#ifndef NESL2C_AST_DIV_H
#define NESL2C_AST_DIV_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class Div : public ArithmeticOperation
{
 
public:
  Div(Node*, Node*);

  void Accept(Visitor*);

};


} // end namespace nesl2c

#endif // NESL2C_AST_DIV_H
