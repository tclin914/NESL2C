%{
//===- nesl.y -----------------------------------------------------===//
//
//  Tsung-Chun Lin <tclin914@gmail.com>
//
//  Copyright (C) 2017, Programming Language and System Lab
//
//===--------------------------------------------------------------===//

#include "nesl2c/AST/OpCode.h"

#include "nesl2c/AST/Node.h"
// Nulary Node
#include "nesl2c/AST/Identifier.h"
#include "nesl2c/AST/TypeNode.h"
#include "nesl2c/AST/ConstantBoolean.h"
#include "nesl2c/AST/ConstantInteger.h"
#include "nesl2c/AST/ConstantFloat.h"
#include "nesl2c/AST/ConstantString.h"
// Unary Node
#include "nesl2c/AST/Goal.h"
#include "nesl2c/AST/UnaryOpFactory.h"
// Binary Node
#include "nesl2c/AST/TopLevels.h"
#include "nesl2c/AST/Assign.h"
#include "nesl2c/AST/Let.h"
#include "nesl2c/AST/ExpBinds.h"
#include "nesl2c/AST/Tuple.h"
#include "nesl2c/AST/LogicOpFactory.h"
#include "nesl2c/AST/RelationalOpFactory.h"
#include "nesl2c/AST/ArithmeticOpFactory.h"
#include "nesl2c/AST/Upt.h"
#include "nesl2c/AST/SequenceRef.h"
#include "nesl2c/AST/ApplyToEach.h"
#include "nesl2c/AST/EmptySequence.h"
#include "nesl2c/AST/Sequence.h"
#include "nesl2c/AST/FunctionCall.h"
#include "nesl2c/AST/ApplyBody.h"
#include "nesl2c/AST/In.h"
#include "nesl2c/AST/SequenceTail.h"
// Ternary Node
#include "nesl2c/AST/IfElse.h"

using namespace nesl2c;

void yyerror(const char *s);
Node* yyheader;

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern int yylineno;

%}

%token <tokenval> FUNCTION
%token <tokenval> DATATYPE
%token <tokenval> NUMBER
%token <tokenval> ORDINAL
%token <tokenval> LOGICAL
%token <tokenval> ANY
%token <tokenval> IN
%token <tokenval> IF
%token <tokenval> ELSE
%token <tokenval> THEN
%token <tokenval> LET
%token <tokenval> AND
%token <tokenval> NAND
%token <tokenval> NOR
%token <tokenval> XOR
%token <tokenval> OR
%token <tokenval> PI
%token <tokenval> PP

%token <tokenval> RARROW
%token <tokenval> LARROW
%token <tokenval> EQ
%token <tokenval> NE
%token <tokenval> LE
%token <tokenval> GE

%token <tokenval> INT
%token <tokenval> BOOL
%token <tokenval> FLOAT
%token <tokenval> CHAR

%token <tokenval> TRUE_CONST
%token <tokenval> FALSE_CONST
%token <tokenstr> INT_CONST
%token <tokenstr> FLOAT_CONST
%token <tokenstr> STRING_CONST 
%token <tokenstr> ID

%type <node> id 
%type <node> Goal TopLevels TopLevel FunId FunTypeDef TypeExp PairTypes TypeList
%type <node> Exp IfOrLetExp ExpBinds ExpBind TupleExp TupleRest OrExp 
%type <node> AndExp RelExp AddExp MulExp ExpExp UnExp 
%type <node> SubscriptExp AtomicExp SpecialId ApplyBody RBinds RBind
%type <node> SequenceTail Const 

%type <opcode> AddOp MulOp RelOp OrOp AndOp UnOp

%left ','
%left OR NOR XOR
%left AND NAND
%left EQ NE '<' '>' LE GE
%left '+' '-' PP LARROW
%left '*' '/' RARROW
%left '^' 
%left '#' '@' 
%nonassoc UMINUS

%union {
  int tokenval;
  char *tokenstr;
  nesl2c::OpCode opcode;
  Node* node;
}

%start Goal

%%

Goal
  : TopLevels
    {
      yyheader = new Goal($1);
    }
  ;

TopLevels
  : TopLevel 
    {
      $$ = $1;
    }
  | TopLevels TopLevel 
    {
      $$ = new TopLevels($1, $2);
    }
  ;

TopLevel 
  : FUNCTION FunId Exp ':' FunTypeDef '=' Exp EndMark
    {
      // TODO:
    }
  | FUNCTION FunId Exp '=' Exp EndMark
    {
      // TODO:
    }
  | DATATYPE ID '(' TypeList ')' EndMark
    {
      // TODO:
    }
  | Exp '=' Exp EndMark 
    {
      $$ = new Assign($1, $3);
    }
  | Exp EndMark 
    {
      $$ = $1;
    }
  ;

FunId   
  : ID    
    {
      $$ = new Identifier($1);
    }
  | SpecialId
    {
      $$ = $1;
    }
  ;

EndMark 
  : ';' 
  | '$'
  ;

FunTypeDef 
  : TypeExp RARROW TypeExp
    {
      // TODO:
    }
  ;

TypeExp 
  : id 
    {  
      // TODO:
    }
  | ID '(' TypeList ')' 
    {
      // TODO:
    }
  | '[' TypeExp ']' 
    {
      // TODO:
    }
  | '(' PairTypes ')' 
    {
      // TODO:
    }
  ;

PairTypes 
  : TypeExp ',' PairTypes
    {
      // TODO:
    }
  | TypeExp 
    {
      $$ = $1;   
    }
  ;

TypeList
  : TypeList ',' TypeExp 
    {
      // TODO:
    }
  | TypeExp
    {
      // TODO:
    }
  ;

Exp 
  : IfOrLetExp 
    {
      $$ = $1;
    }
  | TupleExp 
    {
      $$ = $1;
    }
  ;

IfOrLetExp
  : IF Exp THEN Exp ELSE Exp 
    {
      $$ = new IfElse($2, $4, $6);
    }
  | LET ExpBinds ';' IN Exp 
    {
      $$ = new Let($2, $5);
    }
  | LET ExpBinds IN Exp
    {
      $$ = new Let($2, $4);
    }
  ;

ExpBinds
  : ExpBind 
    {
      $$ = $1;
    }
  | ExpBinds ';' ExpBind 
    { 
      $$ = new ExpBinds($1, $3);
    }
  ;

ExpBind
  : Exp '=' Exp
    {
      $$ = new Assign($1, $3);
    }
  ;

TupleExp
  : OrExp 
    {
      $$ = $1;
    }
  | OrExp ',' TupleRest 
    {
      $$ = new Tuple($1, $3);
    }
  ;


TupleRest
  : TupleExp
    {
      $$ = $1;
    }
  | IfOrLetExp
    {
      $$ = $1;
    }
  ;

OrExp
  : OrExp OrOp AndExp 
    {
      $$ = LogicOpFactory::CreateLogicOpNode($2, $1, $3);
    }
  | AndExp 
    {
      $$ = $1;
    }
  ;

OrOp
  : OR  
    { 
      $$ = OR_OP;
    }
  | NOR 
    { 
      $$ = NOR_OP;
    }
  | XOR 
    { 
      $$ = XOR_OP;
    } 
  ;

AndExp
  : RelExp 
    {
      $$ = $1;
    }
  | AndExp AndOp RelExp 
    {
      $$ = LogicOpFactory::CreateLogicOpNode($2, $1, $3);
    }
  ;

AndOp
  : AND  
    {
      $$ = AND_OP;
    }
  | NAND 
    {
      $$ = NAND_OP;
    }
  ;

RelExp
  : AddExp 
    {
      $$ = $1; 
    }
  | RelExp RelOp AddExp
    {
      $$ = RelationalOpFactory::CreateRelationalOpNode($2, $1, $3);
    }
  ;

RelOp
  : EQ 
    {
      $$ = EQ_OP;
    }
  | NE  
    {
      $$ = NE_OP;
    }
  | '<' 
    {
      $$ = LT_OP;
    }
  | '>' 
    {
      $$ = GT_OP;
    }
  | LE  
    {
      $$ = LE_OP;
    }
  | GE  
    {
      $$ = GE_OP;
    }
  ;

AddExp
  : MulExp 
    {
      $$ = $1;
    }
  | AddExp AddOp MulExp 
    {
      $$ = ArithmeticOpFactory::CreateArithmeticOpNode($2, $1, $3);
    }
  ;

AddOp
  : '+'     
    {
      $$ = ADD_OP;
    }
  | '-'     
    {
      $$ = SUBTRACT_OP;      
    }
  | PP      
    {
      $$ = PP_OP;
    }
  | LARROW  
    {
      $$ = LARROW_OP;
    }
  ;

MulExp
  : ExpExp 
    {
      $$ = $1;
    }
  | MulExp MulOp ExpExp 
    {
      $$ = ArithmeticOpFactory::CreateArithmeticOpNode($2, $1, $3);
    }
  ;

MulOp 
  : '*'    
    {
      $$ = MUL_OP;
    } 
  | '/'    
    {
      $$ = DIV_OP;
    }
  | RARROW 
    {
      $$ = RARROW_OP
    }  
  ;

ExpExp 
  : UnExp 
    { 
      $$ = $1;
    }
  | ExpExp '^' UnExp 
    {
      $$ = new Upt($1, $3);
    }
  ;

UnExp 
  : SubscriptExp 
    {
      $$ = $1;
    }
  | UnOp UnExp   
    {
      $$ = UnaryOpFactory::CreateUnaryOpNode($1, $2);
    }
  ;

UnOp
  : '#'   
    {   
      $$ = SHARP_OP;
    }
  | '@'   
    {
      $$ = AT_OP;
    }
  | '-'   
    {
      $$ = UMINUS_OP
    } 
  ;

SubscriptExp
  : AtomicExp 
    {
      $$ = $1;
    }
  | AtomicExp '[' Exp ']'
    {
      $$ = new SequenceRef($1, $3);
    }
  ;

AtomicExp
  : Const 
    {
      $$ = $1;
    }
  | SpecialId '(' Exp ')' 
    {
      // TODO:
    }
  | '{' ApplyBody '}' 
    {
      $$ = $2;
    }
  | '{' ApplyBody '|' Exp '}' 
    {
      $$ = new ApplyToEach($2, $4);
    }
  | '[' ']' TypeExp 
    {
      $$ = new EmptySequence($3);
    }
  | '[' Exp SequenceTail ']'
    {
      $$ = new Sequence($2, $3);
    }
  | '(' Exp ')' 
    {
      $$ = $2;
    }
  | ID 
    {
      $$ = new Identifier($1);
    }
  | id '(' Exp ')'
    {
      $$ = new FunctionCall($1, $3);
    }
  ;

SpecialId
  : ANY 
    {
      // TODO:
    }
  ;

ApplyBody
  : Exp ':' RBinds
    {
      $$ = new ApplyBody($1, $3);
    }
  | RBinds 
    {
      $$ = $1;
    }
  ;

RBinds
  : RBinds ';' RBind
    {
      // TODO:
    }
  | RBind 
    {
      $$ = $1;
    }
  ;
    
RBind
  : ID 
    {
      $$ = new Identifier($1);
    }
  | Exp IN Exp
    {
      $$ = new In($1, $3);
    }
  ;

id 
  : ID
    {
      $$ = new Identifier($1);
    } 
  | INT   
    {
      $$ = new TypeNode(INTEGER_T);
    }
  | FLOAT 
    {
      $$ = new TypeNode(FLOAT_T);
    }
  | BOOL  
    {
      $$ = new TypeNode(BOOL_T);
    }
  | CHAR  
    {
      $$ = new TypeNode(CHAR_T);
    }
  ;

SequenceTail
  : ':' Exp 
    {
      $$ = new SequenceTail($2, NULL);
    }
  | ':' Exp ':' Exp 
    {
      $$ = new SequenceTail($2, $4);
    }
  | {
      $$ = new SequenceTail(NULL, NULL);
    } 
  ;

Const
  : INT_CONST 
    {
      $$ = new ConstantInteger($1);
    }
  | FLOAT_CONST
    {
      $$ = new ConstantFloat($1);
    }
  | TRUE_CONST
    {
      $$ = new ConstantBoolean(true);
    }
  | FALSE_CONST
    {
      $$ = new ConstantBoolean(false);
    }
  | STRING_CONST
    {
      $$ = new ConstantString($1);
    }
  ;

%%

void yyerror(char const *s) {
  
  fprintf(stderr, "line %d: %s\n", yylineno, s);
}
