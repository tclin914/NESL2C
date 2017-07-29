//===- CodeGenVisitor.cpp -----------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Constants.h"

#include "nesl2c/Visitor/CodeGenVisitor.h"
#include "nesl2c/AST/Assign.h"
#include "nesl2c/AST/Add.h"
#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/AST/ConstantFloat.h"
#include "nesl2c/AST/ConstantBoolean.h"

using namespace nesl2c;

CodeGenVisitor::CodeGenVisitor()
  : m_Module(NULL), m_CurrentFunc(NULL), m_CurrentBB(NULL) {

  m_Module = new Module("NESL2C", m_Context);
}

CodeGenVisitor::~CodeGenVisitor()
{
  delete m_Module;
}

void CodeGenVisitor::Visit(TopLevels* pNode)
{
}

void CodeGenVisitor::Visit(Assign* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (NULL == m_CurrentFunc) {
    makeMainFunc();
  }

  IRBuilder<> builder(m_CurrentBB);
  NESLType type = PopNESLType(m_NumChildOfBinary);
  Value* operand1 = Pop();
  Value* operand2 = Pop();
  operand1->dump();
  printf("Assign\n");
  operand2->dump();
  printf("Assign\n");
  builder.CreateStore(operand2, operand1);
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

void CodeGenVisitor::Visit(Equal* pNode)
{
}

void CodeGenVisitor::Visit(NotEqual* pNode)
{
}

void CodeGenVisitor::Visit(LessThan* pNode)
{
}

void CodeGenVisitor::Visit(GreaterThan* pNode)
{
}

void CodeGenVisitor::Visit(LessEqual* pNode)
{
}

void CodeGenVisitor::Visit(GreaterEqual* pNode)
{
}

void CodeGenVisitor::Visit(Add* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {

    if (NULL == m_CurrentFunc) {
      makeMainFunc(); 
    }

    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand1 = Pop();
    Value* operand2 = Pop();
    
    bool isConst1 = dyn_cast<Constant>(operand1);
    bool isConst2 = dyn_cast<Constant>(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: {
       
        if (isConst1 && isConst2) {
          inst = builder.CreateAdd(operand1, operand2);
        } else if (!isConst1 && isConst2) {  
          LoadInst* loadInst = builder.CreateLoad(operand1);
          inst = builder.CreateAdd(loadInst, operand2);
        } else if (isConst1 && !isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand2);
          inst = builder.CreateAdd(operand1, loadInst);
        } else { // !isConst1 && !isConst2
          LoadInst* loadInst1 = builder.CreateLoad(operand1);
          LoadInst* loadInst2 = builder.CreateLoad(operand2);
          inst = builder.CreateAdd(loadInst1, loadInst2);
        }
        break;
      }      
      case FLOAT_T: {
        
        if (isConst1 && isConst2) {
          inst = builder.CreateFAdd(operand1, operand2);
        } else if (!isConst1 && isConst2) {  
          LoadInst* loadInst = builder.CreateLoad(operand1);
          inst = builder.CreateFAdd(loadInst, operand2);
        } else if (isConst1 && !isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand2);
          inst = builder.CreateFAdd(operand1, loadInst);
        } else { // !isConst1 && !isConst2
          LoadInst* loadInst1 = builder.CreateLoad(operand1);
          LoadInst* loadInst2 = builder.CreateLoad(operand2);
          inst = builder.CreateFAdd(loadInst1, loadInst2);
        }
        break;
      }
      default:
        // TODO:
        break;
    }
    
    Push(inst);
    PushNESLType(type);
  } else {
    // TODO:
  }
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

void CodeGenVisitor::Visit(Sharp* pNode)
{
}

void CodeGenVisitor::Visit(At* pNode)
{
}

void CodeGenVisitor::Visit(Uminus* pNode)
{
}

void CodeGenVisitor::Visit(ApplyBody* pNode)
{
}

void CodeGenVisitor::Visit(In* pNode)
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

  Push(ConstIntValue);
  PushNESLType(INTEGER_T);
}

void CodeGenVisitor::Visit(ConstantFloat* pNode)
{
  Constant *ConstFPValue = ConstantFP::get(Type::getFloatTy(m_Context), 
        pNode->GetFPValue());

  Push(ConstFPValue);
  PushNESLType(FLOAT_T);
}

void CodeGenVisitor::Visit(ConstantBoolean* pNode)
{
  ConstantInt *ConstBoolValue;
  if (pNode->GetBoolValue()) 
    ConstBoolValue = ConstantInt::getTrue(m_Context);
  else
    ConstBoolValue = ConstantInt::getFalse(m_Context);
  
  Push(ConstBoolValue);
  PushNESLType(BOOL_T);
}

void CodeGenVisitor::Visit(ConstantString* pNode)
{
}
