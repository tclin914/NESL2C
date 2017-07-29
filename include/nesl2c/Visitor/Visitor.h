//===- Visitor.h --------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//
#ifndef NESL2C_VISITOR_VISITOR_H
#define NESL2C_VISITOR_VISITOR_H

namespace nesl2c {

class TopLevels;
class Assign;
class EmptySequence;
class Or;
class NotOr;
class XOr;
class And;
class NotAnd;
class Equal;
class NotEqual;
class LessThan;
class GreaterThan;
class LessEqual;
class GreaterEqual;
class Add;
class Subtract;
class PP;
class LARROW;
class Mul;
class Div;
class RARROW;
class Sharp;
class At;
class Uminus;
class Identifier;
class TypeNode;
class SequenceTail;
class ConstantInteger;
class ConstantFloat;
class ConstantBoolean;
class ConstantString;

class Visitor 
{

public:

  virtual void Visit(TopLevels*) = 0;
  virtual void Visit(Assign*) = 0;
  virtual void Visit(EmptySequence*) = 0;

  virtual void Visit(Or*) = 0;
  virtual void Visit(NotOr*) = 0;
  virtual void Visit(XOr*) = 0;
  virtual void Visit(And*) = 0;
  virtual void Visit(NotAnd*) = 0;

  virtual void Visit(Equal*) = 0;
  virtual void Visit(NotEqual*) = 0;
  virtual void Visit(LessThan*) = 0;
  virtual void Visit(GreaterThan*) = 0;
  virtual void Visit(LessEqual*) = 0;
  virtual void Visit(GreaterEqual*) = 0;

  virtual void Visit(Add*) = 0;
  virtual void Visit(Subtract*) = 0;
  virtual void Visit(PP*) = 0;
  virtual void Visit(LARROW*) = 0;
  virtual void Visit(Mul*) = 0;
  virtual void Visit(Div*) = 0;
  virtual void Visit(RARROW*) = 0;

  virtual void Visit(Sharp*) = 0;
  virtual void Visit(At*) = 0;
  virtual void Visit(Uminus*) = 0;

  virtual void Visit(SequenceTail*) = 0;

  virtual void Visit(Identifier*) = 0;
  virtual void Visit(TypeNode*) = 0;
  virtual void Visit(ConstantInteger*) = 0;
  virtual void Visit(ConstantFloat*) = 0;
  virtual void Visit(ConstantBoolean*) = 0;
  virtual void Visit(ConstantString*) = 0;

};

} // namespace of nesl2c

#endif // NESL2C_VISITOR_VISITOR_H
