#ifndef NESL2C_AST_TYPE_NODE_H
#define NESL2C_AST_TYPE_NODE_H

#include "NullaryNode.h"

namespace nesl2c {

class TypeNode : public NullaryNode
{

public:
  TypeNode(Type);
  
  bool Initialize();
  void Accept(Visitor*);

};

} // end namespace nesl2c

#endif // NESL2C_AST_TYPE_NODE_H
