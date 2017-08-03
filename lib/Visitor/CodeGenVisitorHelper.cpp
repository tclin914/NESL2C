//===- CodeGenVisitorHelper.h -------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"

#include "nesl2c/Visitor/CodeGenVisitor.h"
#include "nesl2c/AST/Node.h"

using namespace nesl2c;

void CodeGenVisitor::VisitChildren(Node* pNode, int pNum) 
{
  for (int i = 0; i < pNum; ++i) {
    if (NULL != pNode->GetChild(i))
      pNode->GetChild(i)->Accept(this); 
  }
}

void CodeGenVisitor::Push(Value* pValue) {
  m_Values.push_back(pValue);
}

Value* CodeGenVisitor::Pop() 
{
  Value *value = m_Values.back();
  m_Values.pop_back();
  return value;
}

void CodeGenVisitor::PushNESLType(NESLType pType) {
  m_Types.push_back(pType);
}

NESLType CodeGenVisitor::PopNESLType(int pNum) 
{  
  NESLType type = m_Types.back();
  for (int i = 0; i < pNum; i++) {
    m_Types.pop_back();
  }
  return type;
}

Value* CodeGenVisitor::Dereference(Value* pValue) {
  if (dyn_cast<Constant>(pValue)) 
    return pValue;
  return new LoadInst(pValue, "", false, m_CurrentBB);
}

Type* CodeGenVisitor::ToLLVMType(NESLType pNESLType) {
  
  switch (pNESLType) {
    case VOID_T:
      return Type::getVoidTy(m_Context);
      break;
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
