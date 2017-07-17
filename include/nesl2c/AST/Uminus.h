//===- Uminus.h ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_UMINUS_H
#define NESL2C_AST_UMINUS_H

#include "nesl2c/AST/UnaryOperation.h"

namespace nesl2c {

class Uminus : public UnaryOperation
{

public:
  Uminus(Node*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_UMINUS_H
