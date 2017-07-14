#ifndef NESL2C_OR_H
#define NESL2C_OR_H

#include "LogicOperation.h"

namespace nesl2c {

class Or : public LogicOperation
{

public:
  Or(Node*, Node*);

  void Accept(Visitor*);
    
};

} // namespace of nesl2c

#endif
