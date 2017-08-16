//===- NESLType.h -------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_NESL_TYPE_H
#define NESL2C_AST_NESL_TYPE_H

namespace nesl2c {

enum NESLType
{
  UNDEFINED,
  VOID_T,
  INTEGER_T,
  FLOAT_T,
  BOOL_T,
  CHAR_T,
  STRING_T
};

} // namespace of nesl2c

#endif // NESL2C_AST_NESL_TYPE_H
