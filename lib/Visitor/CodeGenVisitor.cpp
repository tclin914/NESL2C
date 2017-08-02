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
#include "nesl2c/AST/TopLevels.h"
#include "nesl2c/AST/Assign.h"
#include "nesl2c/AST/Add.h"
#include "nesl2c/AST/Subtract.h"
#include "nesl2c/AST/Mul.h"
#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/AST/ConstantFloat.h"
#include "nesl2c/AST/ConstantBoolean.h"

using namespace nesl2c;

CodeGenVisitor::CodeGenVisitor()
  : m_Module(new Module("NESL2C", m_Context)), 
    m_CurrentFunc(cast<Function>(m_Module->getOrInsertFunction("main", ToLLVMType(VOID_T), (Type*)0))), 
    m_CurrentBB(BasicBlock::Create(m_Context, "entryBlock", m_CurrentFunc)) {

}

CodeGenVisitor::~CodeGenVisitor()
{
  delete m_Module;
}

void CodeGenVisitor::Visit(TopLevels* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);
}

void CodeGenVisitor::Visit(Assign* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  IRBuilder<> builder(m_CurrentBB);
  NESLType type = PopNESLType(m_NumChildOfBinary);
  Value* operand1 = Pop();
  Value* operand2 = Pop();
  builder.CreateStore(operand2, operand1);
}

void CodeGenVisitor::Visit(IfElse* pNode)
{
}

void CodeGenVisitor::Visit(Let* pNode)
{
}

void CodeGenVisitor::Visit(ExpBinds* pNode)
{
}

void CodeGenVisitor::Visit(Tuple* pNode)
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
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
  
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    bool isConst1 = dyn_cast<Constant>(operand1);
    bool isConst2 = dyn_cast<Constant>(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: {
      
        if (isConst1 && isConst2) {
          inst = builder.CreateSub(operand1, operand2);
        } else if (!isConst1 && isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand1);
          inst = builder.CreateSub(loadInst, operand2);
        } else if (isConst1 && !isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand2);
          inst = builder.CreateSub(operand1, loadInst);
        } else { // !isConst1 && !isConst2
          LoadInst* loadInst1 = builder.CreateLoad(operand1);
          LoadInst* loadInst2 = builder.CreateLoad(operand2);
          inst = builder.CreateAdd(loadInst1, loadInst2);
        }
        break;
      }
      case FLOAT_T: {
          
        if (isConst1 && isConst2) {
          inst = builder.CreateFSub(operand1, operand2);
        } else if (!isConst1 && isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand1);
          inst = builder.CreateFSub(loadInst, operand2);
        } else if (isConst1 && !isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand2);
          inst = builder.CreateFSub(operand1, loadInst);
        } else { // !isConst1 && !isConst2
          LoadInst* loadInst1 = builder.CreateLoad(operand1);
          LoadInst* loadInst2 = builder.CreateLoad(operand2);
          inst = builder.CreateFSub(loadInst1, loadInst2);
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

void CodeGenVisitor::Visit(PP* pNode)
{
}

void CodeGenVisitor::Visit(LARROW* pNode)
{
}

void CodeGenVisitor::Visit(Mul* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    bool isConst1 = dyn_cast<Constant>(operand1);
    bool isConst2 = dyn_cast<Constant>(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: {
        
        if (isConst1 && isConst2) {
          inst = builder.CreateMul(operand1, operand2);
        } else if (!isConst1 && isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand1);
          inst = builder.CreateMul(loadInst, operand2);
        } else if (isConst1 && !isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand2);
          inst = builder.CreateMul(operand1, loadInst);
        } else { // !isConst1 && !isConst2
          LoadInst* loadInst1 = builder.CreateLoad(operand1);
          LoadInst* loadInst2 = builder.CreateLoad(operand2);
          inst = builder.CreateMul(loadInst1, loadInst2);
        }
        break;               
      }
      case FLOAT_T: {
        
        if (isConst1 && isConst2) {
          inst = builder.CreateFMul(operand1, operand2);
        } else if (!isConst1 && isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand1);
          inst = builder.CreateFMul(loadInst, operand2);
        } else if (isConst1 && !isConst2) {
          LoadInst* loadInst = builder.CreateLoad(operand2);
          inst = builder.CreateFMul(operand1, loadInst);
        } else { // !isConst1 && !isConst2
          LoadInst* loadInst1 = builder.CreateLoad(operand1);
          LoadInst* loadInst2 = builder.CreateLoad(operand2);
          inst = builder.CreateFMul(loadInst1, loadInst2);
        }
        break;            
      }
      default:
        // TODO:
        break;
    }
    inst->dump();
    Push(inst);
    PushNESLType(type);
  } else {
    // TODO: 
  }
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

void CodeGenVisitor::Visit(Upt* pNode)
{
}

void CodeGenVisitor::Visit(SequenceRef* pNode)
{
}

void CodeGenVisitor::Visit(ApplyToEach* pNode)
{
}

void CodeGenVisitor::Visit(EmptySequence* pNode)
{
}

void CodeGenVisitor::Visit(Sequence* pNode)
{
}

void CodeGenVisitor::Visit(FunctionCall* pNode)
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
