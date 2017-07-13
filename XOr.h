#ifndef NESL2C_XOR_H
#define NESL2C_XOR_H

#include "LogicOperation.h"

namespace nesl2c {

class XOr : public LogicOperation
{

public:
  XOr(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
