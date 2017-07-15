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

private:
  string str;

};

} // end namespace nesl2c

#endif // NESL2C_AST_IDENTIFIER_H
