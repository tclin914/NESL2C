#ifndef __TYPE_H___
#define __TYPE_H___

#define VALUE_INVALID   0
#define VALUE_I_VALID   1
#define VALUE_R_VALID   2

#define NODE_TOKEN          1
#define NODE_OP             2
#define NODE_MARK           22

// typeToken
#define TOKE_ID             0
#define TOKE_INT            1
#define TOKE_FLOAT          2
#define TOKE_BOOL           3
#define TOKE_CHAR           4
#define TOKE_STRING         5

#define NODE_INT            3
#define NODE_FLOAT          4
#define NODE_BOOL           5
#define NODE_CHAR           6

// NODE_TYPE 
#define NODE_TYPE_INT       7
#define NODE_TYPE_FLOAT     8
#define NODE_TYPE_CHAR      9
#define NODE_TYPE_BOOL      10

// 
#define NODE_TYPE_PARM      11
#define NODE_SYM_REF        12
#define NODE_TYPE_SEQ       13
#define NODE_SEQ_REF        14
#define NODE_TYPE_PAIR      34

//lazy man
#define NODE_NESL           15
#define NODE_EXP            16
#define NODE_SEQ            17
#define NODE_SEQTAIL        18
#define NODE_IFSTMT         19
#define NODE_THENSTMT       20
#define NODE_ELSESTMT       21
#define NODE_DATATYPE       23
#define NODE_TUPLE          24
#define NODE_BIND           25
#define NODE_PATTERN        26
#define NODE_LET            27 
#define NODE_APPLYBODY1     28
#define NODE_BODY           29
#define NODE_SIEVE          30
#define NODE_EMPSEQ         31
#define NODE_FILTER         32  
#define NODE_IN             33
#define NODE_DEL            36
#define NODE_RBINDS         37
#define NODE_APPLYBODY2     38
#define NODE_APPLYBODY3     39
#define NODE_APPLYBODY4     40
#define NODE_STRING         41
#define NODE_PAIR           42
#define NODE_NEW_SEQ        43
#define PARAM_TUPLE         44
#define LHS_TUPLE           45
#define RHS_TUPLE           46






#define NOTE_IMPLEMENT      49

#define NODE_LIST           50
#define NODE_IFELSE         52

#define NODE_INFER          53
#define NODE_TYPEEXP        54
#define NODE_APPEACH        55

#define GEN_APP2            56
#define GEN_APP3            57
#define GEN_SEQ_REF         58
#define NODE_SEQ_TUPLE      59
#define RB_TUPLE            60
#define ELEM_TUPLE          61

#define NODE_FreeVars       63
#define NODE_APPRET         64
#define NODE_ACTION_TUPLE   65
#define NODE_LETRET         66

#define NODE_FUNC           94
#define NODE_FUNC_CALL      95
#define NODE_FUNC_INPUT     96
#define NODE_FUNC_TYPE      97

#define NODE_VAR_OR_PROC    99
#define NODE_ERROR          100
#define NODE_EMPTY          101

#define OP_ADD            1
#define OP_SUB            2
#define OP_MUL            3
#define OP_DIV            4
#define OP_GT             5
#define OP_LT             6
#define OP_EQ             7
#define OP_GE             8
#define OP_LE             9
#define OP_NE             10
#define OP_NOT            11
#define OP_OR             12
#define OP_COMMA          13
#define OP_AND            14
#define OP_NOR            15
#define OP_NAND           16
#define OP_XOR            17
#define OP_LARROW         20
#define OP_RARROW         21
#define OP_UPT            22
#define OP_SHARP          23
#define OP_AT             24
#define OP_UMINUS         25
#define OP_BIND           26
#define OP_PP             27


// TODO 
// 1. add TypeArray when detect '[]' stuff 
// 2. use the most outside type to represent the node.
// 3. add TypeChar for char or string.
enum StdType {TypeInt=1, TypeFloat, TypeChar, TypeBool, 
              TypeSEQ_I,TypeSEQ_F,TypeSEQ_C,TypeSEQ_B,TypeSEQ,
              TypeTuple_I,TypeTuple_F,TypeTuple_C,TypeTuple_B,
              TypeTuple_IF, TypeTuple_FI, TypeTuple_SF,TypeTuple_FS,
              TypeTuple_SI,TypeTuple_IS,TypeTuple};



#endif
