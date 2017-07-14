#ifndef NESL2C_AST_UMINUS_H
#define NESL2C_AST_UMINUS_H

#include "UnaryOperation.h"

namespace nesl2c {

class Uminus : public UnaryOperation
{

public:
  Uminus(Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_UMINUS_H
