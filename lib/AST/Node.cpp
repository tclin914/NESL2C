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

Node::Node() : f_ID("")
{
  f_LineNo = yylineno; // line_no is a global variable from yacc
}

NESLType Node::GetNESLType()
{
  return f_NESLType; 
}

void Node::SetNESLType(NESLType pType)
{
  f_NESLType = pType;
}

Node* Node::GetChild(int pIndex) 
{
  return f_Children[pIndex];
}

int Node::GetChildNum()
{
  return f_Children.size(); 
}

string Node::GetID() const
{
  return f_ID;
}

bool Node::isLeafNode() const
{
  return f_Children.empty(); 
}

bool Node::isNumber() const
{
  return f_NESLType == INTEGER_T || 
      f_NESLType == FLOAT_T;
}

bool Node::isLogic() const
{
  return f_NESLType == INTEGER_T ||
      f_NESLType == BOOL_T;
}

bool Node::isOrdinal() const 
{
  return isNumber() || f_NESLType == CHAR_T;
}
