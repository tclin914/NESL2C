#ifndef NESL2C_NOT_AND_H
#define NESL2C_NOT_AND_H

#include "LogicOperation.h"

namespace nesl2c {

class NotAnd : public LogicOperation
{

public:
  NotAnd(Node*, Node*);

};

} // namespace of nesl2c

#endif
