#ifndef NESL2C_AST_XOR_H
#define NESL2C_AST_XOR_H

#include "LogicOperation.h"

namespace nesl2c {

class XOr : public LogicOperation
{

public:
  XOr(Node*, Node*);

  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_XOR_H
