#ifndef NESL2C_CODE_GEN_VISITOR_H
#define NESL2C_CODE_GEN_VISITOR_H

#include "Visitor.h"

namespace nesl2c {

class CodeGenVisitor : public Visitor
{
  CodeGenVisitor();

  virtual void Visit(EmptySequence*);

  virtual void Visit(Or*);
  virtual void Visit(NotOr*);
  virtual void Visit(XOr*);
  virtual void Visit(And*);
  virtual void Visit(NotAnd*);

  virtual void Visit(Add*);
  virtual void Visit(Subtract*);
  virtual void Visit(PP*);
  virtual void Visit(LARROW*);
  virtual void Visit(Mul*);
  virtual void Visit(Div*);
  virtual void Visit(RARROW*);

  virtual void Visit(SequenceTail*);

  virtual void Visit(Identifier*);
  virtual void Visit(TypeNode*);
  virtual void Visit(ConstantInteger*);
  virtual void Visit(ConstantFloat*);
  virtual void Visit(ConstantBoolean*);
  virtual void Visit(ConstantString*);
   
};

} // namespace of nesl2c

#endif
