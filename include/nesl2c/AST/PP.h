//===- PP.h -----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_PP_H
#define NESL2C_AST_PP_H

#include "nesl2c/AST/ArithmeticOperation.h"

namespace nesl2c {

class PP : public ArithmeticOperation
{
  
public:
  PP(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_PP_H
