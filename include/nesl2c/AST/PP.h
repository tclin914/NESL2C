#ifndef NESL2C_AST_PP_H
#define NESL2C_AST_PP_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class PP : public ArithmeticOperation
{
  
public:
  PP(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_PP_H
