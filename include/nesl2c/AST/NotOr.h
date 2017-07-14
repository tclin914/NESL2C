#ifndef NESL2C_AST_NOT_OR_H
#define NESL2C_AST_NOT_OR_H

#include "LogicOperation.h"

namespace nesl2c {

class NotOr : public LogicOperation
{

public:
  NotOr(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_NOT_OR_H
