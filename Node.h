#ifndef NESL2C_NODE_H
#define NESL2C_NODE_H

#include <vector>

#include "Type.h"

namespace nesl2c {

class Node 
{

public:
  Node();

  virtual Type GetType();
  Node* GetChild(int);

protected:
  // virtual void ErrorReport(string, ...);
  std::vector<Node*> children;
  int lineNo;
  Type type;
};

} // namespace of nesl2c

#endif
