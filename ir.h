#ifndef __IR_H__
#define __IR_H__

#include "c_type.h"

typedef struct value {
    union {
        int int_val;
        float float_val;
        char *str_val;
    };
} value;

typedef struct declaration {
    
    char *name;
    char *type;

} declatation;

typedef struct op_expression {
  
    char *name;
    C_OP op;
} op_expression;

typedef struct statement {

    char *name;
    char *code;

} statement;

typedef struct ifelse_statement {
    
    char *name;

} ifelse_statement;

typedef struct parameter {

    char *name;
    char *type;

} parameter;

typedef struct function {
  
    char *name;

    parameter *paras;
    statement *states;

} function;

typedef struct module {
    
    char *name;

    function *funcs;

} module;

#endif
