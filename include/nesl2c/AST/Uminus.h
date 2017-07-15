#ifndef NESL2C_AST_UMINUS_H
#define NESL2C_AST_UMINUS_H

#include "nesl2c/AST/UnaryOperation.h"

namespace nesl2c {

class Uminus : public UnaryOperation
{

public:
  Uminus(Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_UMINUS_H
