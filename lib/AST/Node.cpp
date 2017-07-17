//===- Node.cpp ---------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/AST/Node.h"

extern int yylineno;

using namespace nesl2c;

Node::Node()
{
  m_LineNo = yylineno; // line_no is a global variable from yacc
}

NESLType Node::GetType()
{
  return m_NESLType; 
}
