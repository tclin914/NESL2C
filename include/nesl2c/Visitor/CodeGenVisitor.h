//===- CodeGenVisitor.h -------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_VISITOR_CODE_GEN_VISITOR_H
#define NESL2C_VISITOR_CODE_GEN_VISITOR_H

#include <vector>

#include "llvm/IR/LLVMContext.h"

#include "nesl2c/Visitor/Visitor.h"
#include "nesl2c/AST/NESLType.h"
#include "nesl2c/AST/SymbolTable.h"

using namespace std;
using namespace llvm;

namespace llvm {

class Type;
class BasicBlock;
class Value;
}

namespace nesl2c {

class Node;

class CodeGenVisitor : public Visitor
{

public:
  CodeGenVisitor(SymbolTable*);
  ~CodeGenVisitor();

  virtual void Visit(Goal*);
  virtual void Visit(TopLevels*);
  virtual void Visit(Assign*);
  virtual void Visit(IfElse*);
  virtual void Visit(Let*);
  virtual void Visit(ExpBinds*);
  virtual void Visit(Tuple*);

  virtual void Visit(Or*);
  virtual void Visit(NotOr*);
  virtual void Visit(XOr*);
  virtual void Visit(And*);
  virtual void Visit(NotAnd*);

  virtual void Visit(Equal*);
  virtual void Visit(NotEqual*);
  virtual void Visit(LessThan*);
  virtual void Visit(GreaterThan*);
  virtual void Visit(LessEqual*);
  virtual void Visit(GreaterEqual*);

  virtual void Visit(Add*);
  virtual void Visit(Subtract*);
  virtual void Visit(PP*);
  virtual void Visit(LARROW*);
  virtual void Visit(Mul*);
  virtual void Visit(Div*);
  virtual void Visit(RARROW*);

  virtual void Visit(Sharp*);
  virtual void Visit(At*);
  virtual void Visit(Uminus*);

  virtual void Visit(Upt*);
  virtual void Visit(SequenceRef*);
  virtual void Visit(ApplyToEach*);
  virtual void Visit(EmptySequence*);
  virtual void Visit(Sequence*);
  virtual void Visit(FunctionCall*);
  virtual void Visit(ApplyBody*);
  virtual void Visit(In*);
  virtual void Visit(SequenceTail*);

  virtual void Visit(Identifier*);
  virtual void Visit(TypeNode*);
  virtual void Visit(ConstantInteger*);
  virtual void Visit(ConstantFloat*);
  virtual void Visit(ConstantBoolean*);
  virtual void Visit(ConstantString*);

  void verifyModule();
  void dump();

private:
  typedef vector<Value*> Values;
  typedef vector<NESLType> Types;

private:
  void Push(Value*);
  Value* Pop();
  void PushNESLType(NESLType);
  NESLType PopNESLType();
  NESLType PopNESLType(int);

  Value* Dereference(Value*);
  
  // translate NESL Type to LLVM Type
  Type* ToLLVMType(NESLType);

  void transAssign(Node*, Node*);

  Values ReversedOrderPop(int);
  Types ReversedOrderPopNESLType(int);
  int GetDepth(Node*);

private:
  // LLVM IR Container
  LLVMContext m_Context;
  Module* m_Module;
  Function* m_CurrentFunc;
  BasicBlock* m_CurrentBB;

  // stack 
  Values m_Values;
  Types m_Types;

  // symbol table
  SymbolTable* m_SymbolTable;

};

} // namespace of nesl2c

#endif // NESL2C_VISITOR_CODE_GEN_VISITOR_H
