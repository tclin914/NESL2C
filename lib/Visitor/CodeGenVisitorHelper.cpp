//===- CodeGenVisitorHelper.h -------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Verifier.h"

#include "nesl2c/Visitor/CodeGenVisitor.h"
#include "nesl2c/AST/Node.h"
#include "nesl2c/AST/Symbol.h"

using namespace nesl2c;

void CodeGenVisitor::dump()
{
  m_Module->dump();
}

void CodeGenVisitor::verifyModule()
{
  llvm::verifyModule(*m_Module);
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

void CodeGenVisitor::PushNESLType(NESLType pType) 
{
  m_Types.push_back(pType);
}

NESLType CodeGenVisitor::PopNESLType() 
{  
  NESLType type = m_Types.back();
  m_Types.pop_back();
  return type;
}

NESLType CodeGenVisitor::PopNESLType(int pNum) 
{  
  NESLType type = m_Types.back();
  for (int i = 0; i < pNum; ++i)
    m_Types.pop_back();
  return type;
}

Value* CodeGenVisitor::Dereference(Value* pValue) 
{
  if (dyn_cast<Constant>(pValue)) 
    return pValue;
  return new LoadInst(pValue, "", false, m_CurrentBB);
}

Type* CodeGenVisitor::ToLLVMType(NESLType pNESLType) 
{  
  switch (pNESLType) {
    case VOID_T:
      return Type::getVoidTy(m_Context);
      break;
    case INTEGER_T:
      return Type::getInt32Ty(m_Context);
      break;
    case FLOAT_T:
      return Type::getFloatTy(m_Context);
      break;
    case BOOL_T:
      return Type::getInt1Ty(m_Context);
      break;
    case CHAR_T:
    case STRING_T:
    default:
      return NULL;
      break;
  }
}

void CodeGenVisitor::transAssign(Node* pL, Node* pR)
{
  if (nullptr == pL)
      return;

  if (pL->isLeafNode()) {
    // get the number of rvalue
    int stackSize = m_Values.size();
    VisitSelf(pR);
    int numPacked = m_Values.size() - stackSize;
    
    IRBuilder<> builder(m_CurrentBB);
    if (numPacked > 1) {
      // create struct type for packing tuple elements
      StructType* packedType = StructType::create(m_Module->getContext(),
        StringRef(pL->GetID() + "_TYPE").upper());
      vector<Type*> elementTypes;
      for (int i = 0; i < numPacked; ++i) {
        elementTypes.push_back(ToLLVMType(PopNESLType()));
      }
      std::reverse(elementTypes.begin(), elementTypes.end());
      packedType->setBody(elementTypes, false);

      // create a struct and assign data into it
      Value* packedValue = builder.CreateAlloca(packedType, NULL, pL->GetID());
      for (int i = numPacked - 1; i >= 0; --i) {
        Value* elementPtr = builder.CreateStructGEP(packedType, packedValue, i);
        builder.CreateStore(Pop(), elementPtr);
      }
      
      m_SymbolTable->getSymbol(pL->GetID())->SetValue(packedValue);
    } else {
      Type* type = ToLLVMType(PopNESLType());   
      Value* value = builder.CreateAlloca(type, NULL, pL->GetID());
      builder.CreateStore(Pop(), value);

      m_SymbolTable->getSymbol(pL->GetID())->SetValue(value);
    }
  } else {
    
    transAssign(pL->GetChild(0), pR->GetChild(0));
    transAssign(pL->GetChild(1), pR->GetChild(1));
  }
}

CodeGenVisitor::Values CodeGenVisitor::ReversedOrderPop(int pNum) 
{
  Values result;
  for (int i = pNum; i > 0; --i) 
    result.push_back(m_Values[m_Values.size() - i]);
  for (int i = pNum; i > 0; --i)
    Pop();

  return result;
}

CodeGenVisitor::Types CodeGenVisitor::ReversedOrderPopNESLType(int pNum)
{
  Types result;
  for (int i = pNum; i > 0; --i)
    result.push_back(m_Types[m_Types.size() - i]);
  if (pNum > 0)
    PopNESLType(pNum);

  return result;
}

int CodeGenVisitor::GetDepth(Node* pNode)
{
  if (nullptr == pNode)
    return 0;
  
  int maxDepth = 0;
  for (int i = 0; i < pNode->GetChildNum(); ++i) {
    int depth = GetDepth(pNode->GetChild(i));
    maxDepth = depth > maxDepth ? depth : maxDepth;
  }

  return 1 + maxDepth; 
}
