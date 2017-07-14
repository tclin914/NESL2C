#ifndef NESL2C_NODE_H
#define NESL2C_NODE_H

#include <vector>

#include "Type.h"

namespace nesl2c {

class Visitor;

class Node 
{

public:
  Node();
  virtual ~Node() {}

  virtual bool Initialize() = 0;
  virtual bool SemanticCheck() = 0;
  virtual void Accept(Visitor*) = 0;

  virtual Type GetType();

  Node* GetChild(int);

protected:
  // virtual void ErrorReport(string, ...);
  std::vector<Node*> children;
  Type type;
  int lineNo;
};

} // namespace of nesl2c

#endif
