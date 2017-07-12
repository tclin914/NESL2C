#ifndef NESL2C_OPCODE_H
#define NESL2C_OPCODE_H

namespace nesl2c {

enum OpCode {
  ADD_OP,
  SUBTRACT_OP,
  PP_OP,
  LARROW_OP,
  MUL_OP,
  DIV_OP,
  RARROW_OP,

  EQ_OP,
  NE_OP,
  LT_OP,
  GT_OP,
  LE_OP,
  GE_OP,

  AND_OP,
  NAND_OP,
  OR_OP,
  NOR_OP,
  XOR_OP
};

} // namespace of nesl2c

#endif
