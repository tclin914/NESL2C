//===- ArithmeticOperation.h --------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_ARITHMETIC_OPERATION_H
#define NESL2C_AST_ARITHMETIC_OPERATION_H

#include "BinaryNode.h"

namespace nesl2c {

class ArithmeticOperation : public BinaryNode
{

public:
  ArithmeticOperation(Node*, Node*);
  virtual ~ArithmeticOperation() {}

  bool Initialize(); 
  bool SemanticCheck();

private:
  bool locallySemanticCheck();

};

} // namespace of nesl2c

#endif // NESL2C_AST_ARITHMETIC_OPERATION_H
