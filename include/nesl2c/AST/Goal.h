//===- Goal.h -----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_GOAL_H
#define NESL2C_AST_GOAL_H

#include "nesl2c/AST/UnaryNode.h"

namespace nesl2c {

class Goal : public UnaryNode
{

public:
  Goal(Node*);
 
  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif // NESL2C_AST_GOAL_H
