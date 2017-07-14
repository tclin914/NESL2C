#ifndef NESL2C_TYPE_NODE_H
#define NESL2C_TYPE_NODE_H

#include "NullaryNode.h"

namespace nesl2c {

class TypeNode : public NullaryNode
{

public:
  TypeNode(Type);
  
  bool Initialize();
  void Accept(Visitor*);

};

} // namespace of nesl2c

#endif
