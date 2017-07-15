#ifndef NESL2C_AST_SHARP_H
#define NESL2C_AST_SHARP_H

#include "nesl2c/AST/UnaryOperation.h"

namespace nesl2c {

class Sharp : public UnaryOperation
{

public:
  Sharp(Node*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_SHARP_H
