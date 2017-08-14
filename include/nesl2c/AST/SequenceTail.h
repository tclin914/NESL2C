//===- SequenceTail.h ---------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_SEQUENCE_TAIL_H
#define NESL2C_AST_SEQUENCE_TAIL_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class SequenceTail : public BinaryNode
{
  
public:  
  SequenceTail(Node*, Node*);

  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_SEQUENCE_TAIL_H
