//===- CodeGenVisitor.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "llvm/IR/Constants.h"

#include "nesl2c/Visitor/CodeGenVisitor.h"
#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/AST/ConstantBoolean.h"

using namespace nesl2c;

CodeGenVisitor::CodeGenVisitor()
{
}

void CodeGenVisitor::Visit(EmptySequence* pNode)
{
}

void CodeGenVisitor::Visit(Or* pNode)
{
}

void CodeGenVisitor::Visit(NotOr* pNode)
{
}

void CodeGenVisitor::Visit(XOr* pNode)
{
}

void CodeGenVisitor::Visit(And* pNode)
{
}

void CodeGenVisitor::Visit(NotAnd* pNode)
{
}

void CodeGenVisitor::Visit(Add* pNode)
{
  
}

void CodeGenVisitor::Visit(Subtract* pNode)
{
}

void CodeGenVisitor::Visit(PP* pNode)
{
}

void CodeGenVisitor::Visit(LARROW* pNode)
{
}

void CodeGenVisitor::Visit(Mul* pNode)
{
}

void CodeGenVisitor::Visit(Div* pNode)
{
}

void CodeGenVisitor::Visit(RARROW* pNode)
{
}

void CodeGenVisitor::Visit(SequenceTail* pNode)
{
}

void CodeGenVisitor::Visit(Identifier* pNode)
{
}

void CodeGenVisitor::Visit(TypeNode* pNode)
{
}

void CodeGenVisitor::Visit(ConstantInteger* pNode)
{
  // TODO: Be careful with integer is signed or unsigned in nesl
  ConstantInt *ConstIntValue = ConstantInt::get(IntegerType::get(m_Context, 32), 
        pNode->GetIntValue());

  m_Values.push_back(ConstIntValue);
  m_Types.push_back(INTEGER_T);
}

void CodeGenVisitor::Visit(ConstantFloat* pNode)
{
}

void CodeGenVisitor::Visit(ConstantBoolean* pNode)
{
  ConstantInt *ConstBoolValue;
  if (pNode->GetBoolValue()) 
    ConstBoolValue = ConstantInt::getTrue(m_Context);
  else
    ConstBoolValue = ConstantInt::getFalse(m_Context);
 
  m_Values.push_back(ConstBoolValue);
  m_Types.push_back(BOOL_T);
}

void CodeGenVisitor::Visit(ConstantString* pNode)
{
}
