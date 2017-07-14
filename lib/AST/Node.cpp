
#include "nesl2c/AST/Node.h"

extern int yylineno;

using namespace nesl2c;

Node::Node()
{
  lineNo = yylineno; // line_no is a global variable from yacc
}

Type Node::GetType()
{
  return type; 
}
