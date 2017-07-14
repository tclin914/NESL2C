#ifndef NESL2C_PP_H
#define NESL2C_PP_H

#include "ArithmeticOperation.h"

namespace nesl2c {

class PP : public ArithmeticOperation
{
  
public:
  PP(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
