#ifndef NESL2C_AND_H
#define NESL2C_AND_H

#include "LogicOperation.h"

namespace nesl2c {

class And : public LogicOperation
{
  
public:
  And(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
