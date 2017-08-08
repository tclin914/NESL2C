//===- TypeAnalysisVisitor.cpp ------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#include "nesl2c/Visitor/TypeAnalysisVisitor.h"

using namespace nesl2c;

TypeAnalysisVisitor::TypeAnalysisVisitor()
{
}

void TypeAnalysisVisitor::Visit(TopLevels* pNode)
{
}

void TypeAnalysisVisitor::Visit(Assign* pNode)
{
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
}

void TypeAnalysisVisitor::Visit(Subtract* pNode)
{
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
}

void TypeAnalysisVisitor::Visit(TypeNode* pNode)
{
}

void TypeAnalysisVisitor::Visit(ConstantInteger* pNode)
{
}

void TypeAnalysisVisitor::Visit(ConstantFloat* pNode)
{
}

void TypeAnalysisVisitor::Visit(ConstantBoolean* pNode)
{
}

void TypeAnalysisVisitor::Visit(ConstantString* pNode)
{
}
