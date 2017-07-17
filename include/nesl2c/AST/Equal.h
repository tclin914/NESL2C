//===- Equal.h ----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_EQUAL_H
#define NESL2C_AST_EQUAL_H

#include "nesl2c/AST/RelationalOperation.h"

namespace nesl2c {

class Equal : public RelationalOperation
{

public:
  Equal(Node*, Node*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_EQUAL_H
