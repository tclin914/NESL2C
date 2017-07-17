
#include "nesl2c/Visitor/CodeGenVisitor.h"

using namespace nesl2c;

void CodeGenVisitor::VisitChildren(Node *pNode, int pNum) {
  for (int i = 0; i < pNum; ++i) {
    if (NULL != pNode->GetChild(i))
      pNode->GetChild(i)->Accept(this);
  }
}

Value *CodeGenVisitor::Pop() {
  Value *value = values.back();
  value.pop_back();
  return value;
}

NESLType CodeGenVisitor::PopNESLType(int num) {
  NESLType type = types.back();
  for (int i = 0; i < num; i++) {
    types.pop_back();
  }
  return type;
}

Type *CodeGenVisitor::ToLLVMType(NESLType type) {
  
  switch (type) {
    case INTEGER_T:
    case FLOAT_T:
    case BOOL_T:
    case CHAR_T:
    case STRING_T:
    default:
      return NULL;
      break;
  }
}
