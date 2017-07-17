//===- LogicOperation.h -------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_LOGIC_OPERATION_H
#define NESL2C_AST_LOGIC_OPERATION_H

#include "nesl2c/AST/BinaryNode.h"

namespace nesl2c {

class LogicOperation : public BinaryNode
{

public:
  LogicOperation(Node*, Node*);
  virtual ~LogicOperation() {}

  bool Initialize();
  bool SemanticCheck();

private:
  bool locallySemanticCheck();
                    
};

} // namespace of nesl2c

#endif // NESLC2_AST_LOGIC_OPERATION_H
