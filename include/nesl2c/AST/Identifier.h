//===- Identifier.h -----------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_AST_IDENTIFIER_H
#define NESL2C_AST_IDENTIFIER_H

#include <string>

#include "nesl2c/AST/NullaryNode.h"

using namespace std;

namespace nesl2c {

class Identifier : public NullaryNode 
{

public:
  Identifier(string);
  
  bool Initialize();
  void Accept(Visitor*);

  string getID() const;

private:
  string m_Value;

};

} // namespace of nesl2c

#endif // NESL2C_AST_IDENTIFIER_H
