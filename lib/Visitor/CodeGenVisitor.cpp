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
#include "nesl2c/AST/Symbol.h"
#include "nesl2c/AST/TopLevels.h"
#include "nesl2c/AST/Assign.h"
#include "nesl2c/AST/Tuple.h"
#include "nesl2c/AST/Or.h"
#include "nesl2c/AST/NotOr.h"
#include "nesl2c/AST/XOr.h"
#include "nesl2c/AST/And.h"
#include "nesl2c/AST/NotAnd.h"
#include "nesl2c/AST/Equal.h"
#include "nesl2c/AST/NotEqual.h"
#include "nesl2c/AST/LessThan.h"
#include "nesl2c/AST/GreaterThan.h"
#include "nesl2c/AST/LessEqual.h"
#include "nesl2c/AST/GreaterEqual.h"
#include "nesl2c/AST/Add.h"
#include "nesl2c/AST/Subtract.h"
#include "nesl2c/AST/Mul.h"
#include "nesl2c/AST/Div.h"
#include "nesl2c/AST/Identifier.h"
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
  transAssign(pNode->GetChild(0), pNode->GetChild(1));
  // VisitChildren(pNode, m_NumChildOfBinary);
  
  // IRBuilder<> builder(m_CurrentBB);

  // int leftDepth = GetDepth(pNode->GetChild(0));
  // int rightDepth = GetDepth(pNode->GetChild(1));
  // int numPacked = rightDepth - leftDepth + 1;

  // Symbol* symbol;
  // while (NULL != (symbol = PopSymbol())) {
    // if (numPacked > 0) {
      // // create struct for packing tuple element
      // StructType* packedType = StructType::create(m_Module->getContext(), 
          // StringRef(symbol->getID() + "_type").upper());
      // vector<Type*> elementTypes;
      // while (numPacked > 0) {
        // elementTypes.push_back(ToLLVMType(PopNESLType(0))); 
        // --numPacked;
      // }
      // packedType->setBody(elementTypes, false);       
      
      // // set up data of elements
      // Value* packedValue = builder.CreateAlloca(packedType, NULL, symbol->getID());
      // for (int i = 0; i < packedType->getNumElements(); ++i) {
        // Value* elementPtr = builder.CreateStructGEP(packedType, packedValue, i);
        // builder.CreateStore(Pop(), elementPtr)->dump();
      // }

      // symbol->setValue(packedValue);
    // } else {
    
    // }
  // }
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
  VisitChildren(pNode, m_NumChildOfBinary);
}

void CodeGenVisitor::Visit(Or* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
 
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);
    // INTEGER or BOOL
    Value* inst = builder.CreateOr(operand1, operand2);
    
    Push(inst);
    PushNESLType(type);
  } else {
    // TODO:
  }
}

void CodeGenVisitor::Visit(NotOr* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
  
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);
    // INTEGER or BOOL
    Value* inst = builder.CreateNot(builder.CreateOr(operand1, operand2));
    
    Push(inst);
    PushNESLType(type);
  } else {
    // TODO:
  }
}

void CodeGenVisitor::Visit(XOr* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);
    // INTEGER or BOOL
    Value* inst = builder.CreateXor(operand1, operand2);
    
    Push(inst);
    PushNESLType(type); 
  } else {
    // TODO:
  }
}

void CodeGenVisitor::Visit(And* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);
    // INTEGER or BOOL
    Value* inst = builder.CreateAnd(operand1, operand2);
    
    Push(inst);
    PushNESLType(type);
  }
}

void CodeGenVisitor::Visit(NotAnd* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);
    // INTEGER or BOOL
    Value* inst = builder.CreateNot(builder.CreateAnd(operand1, operand2));
    
    Push(inst);
    PushNESLType(type);
  }
}

void CodeGenVisitor::Visit(Equal* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
  
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T:
        inst = builder.CreateICmpEQ(operand1, operand2);
        break;
      case FLOAT_T: // TODO: OEQ or UEQ
        inst = builder.CreateFCmpUEQ(operand1, operand2);
        break; 
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

void CodeGenVisitor::Visit(NotEqual* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
  
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T:
        inst = builder.CreateICmpNE(operand1, operand2);
        break;
      case FLOAT_T:
        inst = builder.CreateFCmpUNE(operand1, operand2);
        break;
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

void CodeGenVisitor::Visit(LessThan* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T:
        inst = builder.CreateICmpSLT(operand1, operand2);
        break;
      case FLOAT_T: // TODO: OLT or ULT
        inst = builder.CreateFCmpULT(operand1, operand2);
        break;
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

void CodeGenVisitor::Visit(GreaterThan* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T:
        inst = builder.CreateICmpSGT(operand1, operand2);
        break;
      case FLOAT_T: // TODO: OGT or UGT
        inst = builder.CreateFCmpUGT(operand1, operand2);
        break;
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

void CodeGenVisitor::Visit(LessEqual* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T:
        inst = builder.CreateICmpSLE(operand1, operand2);
        break;
      case FLOAT_T: // TODO: OLE or ULE
        inst = builder.CreateFCmpULE(operand1, operand2);
        break;
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

void CodeGenVisitor::Visit(GreaterEqual* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T:
        inst = builder.CreateICmpSGE(operand1, operand2);
        break;
      case FLOAT_T: // TODO: OGE or UGE
        inst = builder.CreateFCmpUGE(operand1, operand2);
        break;
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

void CodeGenVisitor::Visit(Add* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {

    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();
   
    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: 
        inst = builder.CreateAdd(operand1, operand2);
        break;
      case FLOAT_T: 
        inst = builder.CreateFAdd(operand1, operand2);
        break;
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

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: 
        inst = builder.CreateSub(operand1, operand2);
        break;
      case FLOAT_T:   
        inst = builder.CreateFSub(operand1, operand2);
        break;
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

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: 
        inst = builder.CreateMul(operand1, operand2);
        break;               
      case FLOAT_T: 
        inst = builder.CreateFMul(operand1, operand2);
        break;            
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

void CodeGenVisitor::Visit(Div* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (m_Values.size() >= m_NumChildOfBinary) {
    
    IRBuilder<> builder(m_CurrentBB);
    NESLType type = PopNESLType(m_NumChildOfBinary);
    Value* operand2 = Pop();
    Value* operand1 = Pop();

    operand1 = Dereference(operand1);
    operand2 = Dereference(operand2);

    Value* inst;
    switch (type) {
      case INTEGER_T: 
        // TODO: unsigned div or signed div
        inst = builder.CreateUDiv(operand1, operand2);
        break;               
      case FLOAT_T: 
        inst = builder.CreateFDiv(operand1, operand2);
        break;            
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
  Symbol* symbol = new Symbol(pNode->GetID(), UNDEFINED);
  m_SymbolTable.addSymbol(symbol);

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
