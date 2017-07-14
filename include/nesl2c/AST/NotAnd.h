#ifndef NESL2C_AST_NOT_AND_H
#define NESL2C_AST_NOT_AND_H

#include "LogicOperation.h"

namespace nesl2c {

class NotAnd : public LogicOperation
{

public:
  NotAnd(Node*, Node*);
  
  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_NOT_AND_H
