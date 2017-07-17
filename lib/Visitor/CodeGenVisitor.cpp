
#include "llvm/IR/Constants.h"

#include "nesl2c/Visitor/CodeGenVisitor.h"
#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/AST/ConstantBoolean.h"

using namespace nesl2c;

CodeGenVisitor::CodeGenVisitor()
{
}

void CodeGenVisitor::Visit(EmptySequence* node)
{
}

void CodeGenVisitor::Visit(Or* node)
{
}

void CodeGenVisitor::Visit(NotOr* node)
{
}

void CodeGenVisitor::Visit(XOr* node)
{
}

void CodeGenVisitor::Visit(And* node)
{
}

void CodeGenVisitor::Visit(NotAnd* node)
{
}

void CodeGenVisitor::Visit(Add* node)
{
  
}

void CodeGenVisitor::Visit(Subtract* node)
{
}

void CodeGenVisitor::Visit(PP* node)
{
}

void CodeGenVisitor::Visit(LARROW* node)
{
}

void CodeGenVisitor::Visit(Mul* node)
{
}

void CodeGenVisitor::Visit(Div* node)
{
}

void CodeGenVisitor::Visit(RARROW* node)
{
}

void CodeGenVisitor::Visit(SequenceTail* node)
{
}

void CodeGenVisitor::Visit(Identifier* node)
{
}

void CodeGenVisitor::Visit(TypeNode* node)
{
}

void CodeGenVisitor::Visit(ConstantInteger* node)
{
  // TODO: Be careful with integer is signed or unsigned in nesl
  ConstantInt *ConstIntValue = ConstantInt::get(IntegerType::get(Context, 32), 
        node->GetIntValue());

  Values.push_back(ConstIntValue);
  Types.push_back(INTEGER_T);
}

void CodeGenVisitor::Visit(ConstantFloat* node)
{
}

void CodeGenVisitor::Visit(ConstantBoolean* node)
{
  ConstantInt *ConstBoolValue;
  if (node->GetBoolValue()) 
    ConstBoolValue = ConstantInt::getTrue(Context);
  else
    ConstBoolValue = ConstantInt::getFalse(Context);
 
  Values.push_back(ConstBoolValue);
  Types.push_back(BOOL_T);
}

void CodeGenVisitor::Visit(ConstantString* node)
{
}
