#ifndef __TYPE_H___
#define __TYPE_H___

#define VALUE_INVALID   0
#define VALUE_I_VALID   1
#define VALUE_F_VALID   2

enum TokenNum {
    TOKE_ID = 1,   
    TOKE_INT,  
    TOKE_FLOAT,
    TOKE_BOOL, 
    TOKE_CHAR, 
    TOKE_STRING
};

// NODE_TYPE 
enum NodeNum {
    NODE_TOKEN = 1, 
    NODE_OP, 
    NODE_MARK, 
    NODE_INT, 
    NODE_FLOAT, 
    NODE_BOOL, 
    NODE_CHAR, 
    NODE_TYPE_INT, 
    NODE_TYPE_FLOAT, 
    NODE_TYPE_CHAR, 
    NODE_TYPE_BOOL, 
    NODE_TYPE_SEQ, 
    NODE_TYPE_TUPLE,    
    NODE_TYPE_PARM, 
    NODE_SYM_REF, 
    NODE_SEQ_REF, 
    NODE_TYPE_PAIR, 
    NODE_NESL, 
    NODE_EXP, 
    NODE_SEQ, 
    NODE_SEQTAIL, 
    NODE_IFSTMT, 
    NODE_THENSTMT, 
    NODE_ELSESTMT, 
    NODE_DATATYPE, 
    NODE_TUPLE, 
    NODE_BIND, 
    NODE_PATTERN, 
    NODE_LET,  
    NODE_BODY, 
    NODE_SIEVE, 
    NODE_EMPSEQ, 
    NODE_FILTER,   
    NODE_IN, 
    NODE_DEL, 
    NODE_RBINDS, 
    NODE_APPLYBODY1, 
    NODE_APPLYBODY2, 
    NODE_APPLYBODY3, 
    NODE_APPLYBODY4, 
    NODE_STRING, 
    NODE_PAIR, 
    NODE_NEW_SEQ, 
    PARAM_TUPLE, 
    LHS_TUPLE, 
    RHS_TUPLE, 
    NODE_ASSIGN, 
    FPARAM_TUPLE, 
    FPARAM_TOKEN, 
    NOTE_IMPLEMENT, 
    NODE_LIST, 
    NODE_IFELSE, 
    NODE_INFER, 
    NODE_TYPEEXP, 
    NODE_APPEACH, 
    GEN_APP2, 
    GEN_APP3, 
    GEN_SEQ_REF, 
    NODE_SEQ_TUPLE, 
    RB_TUPLE, 
    ELEM_TUPLE, 
    NODE_FreeVars, 
    NODE_APPRET, 
    NODE_ACTION_TUPLE, 
    NODE_LETRET, 
    NODE_FUNC, 
    NODE_FUNC_CALL, 
    NODE_FUNC_INPUT, 
    NODE_FUNC_TYPE, 
    NODE_VAR_OR_PROC, 
    NODE_ERROR, 
    NODE_EMPTY
}; 


enum OpType {
    OP_ADD = 1,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_GT,
    OP_LT,
    OP_EQ,
    OP_GE,
    OP_LE,
    OP_NE,
    OP_NOT,
    OP_OR,
    OP_COMMA,
    OP_AND,
    OP_NOR,
    OP_NAND,
    OP_XOR,
    OP_LARROW,
    OP_RARROW,
    OP_UPT,
    OP_SHARP,
    OP_AT,
    OP_UMINUS,
    OP_BIND,
    OP_PP
};

enum StdType {TYPEINT = 1, TYPEFLOAT, TYPECHAR, TYPEBOOL, TYPESEQ, TYPETUPLE};

struct DataType{
    enum StdType type;
    struct DataType* child1;
    struct DataType* child2;
};

#endif
