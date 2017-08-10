//===- TypeAnalysisVisitor.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "llvm/Support/ErrorHandling.h"

#include "nesl2c/Visitor/TypeAnalysisVisitor.h"
#include "nesl2c/AST/Symbol.h"
#include "nesl2c/AST/TopLevels.h"
#include "nesl2c/AST/Assign.h"
#include "nesl2c/AST/Add.h"
#include "nesl2c/AST/Subtract.h"
#include "nesl2c/AST/Identifier.h"
#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/AST/ConstantFloat.h"
#include "nesl2c/AST/ConstantBoolean.h"
#include "nesl2c/AST/ConstantString.h"

using namespace llvm;
using namespace nesl2c;

TypeAnalysisVisitor::TypeAnalysisVisitor()
{
}

void TypeAnalysisVisitor::Visit(TopLevels* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);
}

void TypeAnalysisVisitor::Visit(Assign* pNode)
{
  VisitAssign(pNode->GetChild(0), pNode->GetChild(1));
}

void TypeAnalysisVisitor::Visit(IfElse* pNode)
{
}

void TypeAnalysisVisitor::Visit(Let* pNode)
{
}

void TypeAnalysisVisitor::Visit(ExpBinds* pNode)
{
}

void TypeAnalysisVisitor::Visit(Tuple* pNode)
{
}

void TypeAnalysisVisitor::Visit(Or* pNode)
{
}

void TypeAnalysisVisitor::Visit(NotOr* pNode)
{
}

void TypeAnalysisVisitor::Visit(XOr* pNode)
{
}

void TypeAnalysisVisitor::Visit(And* pNode)
{
}

void TypeAnalysisVisitor::Visit(NotAnd* pNode)
{
}

void TypeAnalysisVisitor::Visit(Equal* pNode)
{
}

void TypeAnalysisVisitor::Visit(NotEqual* pNode)
{
}

void TypeAnalysisVisitor::Visit(LessThan* pNode)
{
}

void TypeAnalysisVisitor::Visit(GreaterThan* pNode)
{
}

void TypeAnalysisVisitor::Visit(LessEqual* pNode)
{
}

void TypeAnalysisVisitor::Visit(GreaterEqual* pNode)
{
}

void TypeAnalysisVisitor::Visit(Add* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);

  if (pNode->GetChild(0)->isUndefined() || pNode->GetChild(1)->isUndefined())
    report_fatal_error("Type Analysis: The operand of + operation is undefined");

  if (!pNode->GetChild(0)->isNumber() || !pNode->GetChild(1)->isNumber())
    report_fatal_error("Type Analysis: The operand of + operation must be integer or float type");
  
  if (pNode->GetChild(0)->GetNESLType() != pNode->GetChild(1)->GetNESLType())
    report_fatal_error("Type Analysis: The operands of + operation are not the same type");

  pNode->SetNESLType(pNode->GetChild(0)->GetNESLType());
}

void TypeAnalysisVisitor::Visit(Subtract* pNode)
{
  VisitChildren(pNode, m_NumChildOfBinary);
  
  if (pNode->GetChild(0)->isUndefined() || pNode->GetChild(1)->isUndefined())
    report_fatal_error("Type Analysis: The operand of - operation is undefined");

  if (!pNode->GetChild(0)->isNumber() || !pNode->GetChild(1)->isNumber())
    report_fatal_error("Type Analysis: The operand of - operation must be integer or float type");

  if (pNode->GetChild(0)->GetNESLType() != pNode->GetChild(1)->GetNESLType())
    report_fatal_error("Type Analysis: The operands of - operation are not the same type");

  pNode->SetNESLType(pNode->GetChild(0)->GetNESLType());
}

void TypeAnalysisVisitor::Visit(PP* pNode)
{
}

void TypeAnalysisVisitor::Visit(LARROW* pNode)
{
}

void TypeAnalysisVisitor::Visit(Mul* pNode)
{
}

void TypeAnalysisVisitor::Visit(Div* pNode)
{
}

void TypeAnalysisVisitor::Visit(RARROW* pNode)
{
}

void TypeAnalysisVisitor::Visit(Sharp* pNode)
{
}

void TypeAnalysisVisitor::Visit(At* pNode)
{
}

void TypeAnalysisVisitor::Visit(Uminus* pNode)
{
}

void TypeAnalysisVisitor::Visit(Upt* pNode)
{
}

void TypeAnalysisVisitor::Visit(SequenceRef* pNode)
{
}

void TypeAnalysisVisitor::Visit(ApplyToEach* pNode)
{
}

void TypeAnalysisVisitor::Visit(EmptySequence* pNode)
{
}

void TypeAnalysisVisitor::Visit(Sequence* pNode)
{
}

void TypeAnalysisVisitor::Visit(FunctionCall* pNode)
{
}

void TypeAnalysisVisitor::Visit(ApplyBody* pNode)
{
}

void TypeAnalysisVisitor::Visit(In* pNode)
{
}

void TypeAnalysisVisitor::Visit(SequenceTail* pNode)
{
}

void TypeAnalysisVisitor::Visit(Identifier* pNode)
{
  Symbol* symbol = m_SymbolTable.getSymbol(pNode->GetID());
  if (nullptr == symbol) 
    m_SymbolTable.addSymbol(new Symbol(pNode->GetID(), UNDEFINED));
  else
    pNode->SetNESLType(symbol->GetNESLType());
}

void TypeAnalysisVisitor::Visit(TypeNode* pNode)
{
}

void TypeAnalysisVisitor::Visit(ConstantInteger* pNode)
{
  pNode->SetNESLType(INTEGER_T);
}

void TypeAnalysisVisitor::Visit(ConstantFloat* pNode)
{
  pNode->SetNESLType(FLOAT_T);
}

void TypeAnalysisVisitor::Visit(ConstantBoolean* pNode)
{
  pNode->SetNESLType(BOOL_T);
}

void TypeAnalysisVisitor::Visit(ConstantString* pNode)
{
  pNode->SetNESLType(STRING_T);
}

void TypeAnalysisVisitor::VisitAssign(Node* pL, Node* pR) 
{
  if (nullptr == pL)
    return;

  if (pL->isLeafNode()) {
    VisitSelf(pL);
    VisitSelf(pR);

    m_SymbolTable.getSymbol(pL->GetID())->SetNESLType(pR->GetNESLType());
  } else if (pR->isLeafNode()) {
       
    report_fatal_error("Type Analysis: The number of tuple of rvalue is less than lvalue");
  } else {
  
    VisitAssign(pL->GetChild(0), pR->GetChild(0));
    VisitAssign(pL->GetChild(1), pR->GetChild(1));
  }
}
