#ifndef NESL2C_AST_OR_H
#define NESL2C_AST_OR_H

#include "LogicOperation.h"

namespace nesl2c {

class Or : public LogicOperation
{

public:
  Or(Node*, Node*);

  void Accept(Visitor*);
    
};

} // end namespace nesl2c

#endif // NESL2C_AST_OR_H
