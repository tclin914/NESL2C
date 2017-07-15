#ifndef NESL2C_AST_AND_H
#define NESL2C_AST_AND_H

#include "nesl2c/AST/LogicOperation.h"

namespace nesl2c {

class And : public LogicOperation
{
  
public:
  And(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_AND_H
