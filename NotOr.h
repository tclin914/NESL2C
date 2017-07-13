#ifndef NESL2C_NOT_OR_H
#define NESL2C_NOT_OR_H

#include "LogicOperation.h"

namespace nesl2c {

class NotOr : public LogicOperation
{

public:
  NotOr(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
