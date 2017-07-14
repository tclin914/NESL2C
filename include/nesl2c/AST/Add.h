#ifndef NESL2C_AST_ADD_H
#define NESL2C_AST_ADD_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class Add : public ArithmeticOperation
{

public:
  Add(Node*, Node*);
  
  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_ADD_H
