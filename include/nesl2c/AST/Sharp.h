//===- Sharp.h ------------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_SHARP_H
#define NESL2C_AST_SHARP_H

#include "nesl2c/AST/UnaryOperation.h"

namespace nesl2c {

class Sharp : public UnaryOperation
{

public:
  Sharp(Node*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_SHARP_H
