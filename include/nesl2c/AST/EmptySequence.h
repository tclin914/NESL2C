//===- EmptySequence.h --------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_EMPTY_SEQUENCE_H
#define NESL2C_AST_EMPTY_SEQUENCE_H

#include "nesl2c/AST/UnaryNode.h"

namespace nesl2c {

class EmptySequence : public UnaryNode
{

public:
  EmptySequence(Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_EMPTY_SEQUENCE_H
