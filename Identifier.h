#ifndef NESL2C_IDENTIFIER_H
#define NESL2C_IDENTIFIER_H

#include <string>

#include "NullaryNode.h"

using namespace std;

namespace nesl2c {

class Identifier : public NullaryNode 
{

public:
  Identifier(string);

private:
  string str;

};

} // namespace of nesl2c

#endif
