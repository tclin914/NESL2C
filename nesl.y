%{

#include "treenode.h"
#include "treenodetype.h"

#include "lex.yy.c"

int yydebug =1;
int yyerror(const char *s);

treenode *yyheader;
%}

%union {
    int tokenval;
    char *tokenstr;
    int tokenint;
    float tokenfloat;
    treenode *node;
}

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
%token <tokenint> INT_CONST
%token <tokenfloat> FLOAT_CONST
%token <tokenstr> STRING_CONST 
%token <tokenstr> ID

%type <node> id 
%type <node> Goal TopLevels TopLevel FunId FunTypeDef TypeExp PairTypes TypeList
%type <node> Exp IfOrLetExp ExpBinds ExpBind TupleExp TupleRest OrExp OrOp 
%type <node> AndExp AndOp RelExp RelOp AddExp AddOp MulExp MulOp ExpExp UnExp 
%type <node> UnOp SubscriptExp AtomicExp SpecialId ApplyBody RBinds RBind
%type <node> SequenceTail Const 

%left ','
%left OR NOR XOR
%left AND NAND
%left EQ NE '<' '>' LE GE
%left '+' '-' PP LARROW
%left '*' '/' RARROW
%left '^' 
%left '#' '@' 
%nonassoc UMINUS

%start Goal

%%

Goal
    :   TopLevels
        {
            yyheader = $1;
        }
    ;

TopLevels
    :   TopLevel 
        {
            $$ = $1;
        }
    |   TopLevels TopLevel 
        {
            $$ = createNode(NODE_TOP_LEVELS);
            $$->left = $1;
            $$->right = $2;
        }
    ;

TopLevel 
    :   FUNCTION FunId Exp ':' FunTypeDef '=' Exp EndMark
        {
            // TODO:
        }
    |   FUNCTION FunId Exp '=' Exp EndMark
        {
            // TODO:
        }
    |   DATATYPE ID '(' TypeList ')' EndMark
        {
            // TODO:
        }
    |   Exp '=' Exp EndMark 
        {
            $$ = createNode(NODE_ASSIGN);
            $$->left = $1;
            $$->right = $3;
        }
    |   Exp EndMark 
        {
            $$ = $1;
        }
    ;

FunId   
    :   ID    
        {
            $$ = createNode(NODE_ID);
            $$->str_val = $1;
        }
    |   SpecialId
        {
            $$ = $1;
        }
    ;

EndMark 
    :   ';' 
    |   '$'
    ;

FunTypeDef 
    :   TypeExp RARROW TypeExp
        {    
            $$ = createNode(NODE_FUNC_TYPE_DEF);
            $$->left = $1;
            $$->right = $3;
        }
    ;

TypeExp 
    :   id 
        {  
            $$ = $1;
        }
    |   ID '(' TypeList ')' 
        {
            // TODO:
        }
    |   '[' TypeExp ']' 
        {
            // TODO:
        }
    |   '(' PairTypes ')' 
        {
            // TODO:
        }
    ;

PairTypes 
    :   TypeExp ',' PairTypes
        {
            // TODO:
        }
    |   TypeExp 
        {
            $$ = $1;   
        }
    ;

TypeList
    :   TypeList ',' TypeExp 
        {
            // TODO:
        }
    |   TypeExp
        {
            $$ = $1;    
        }
    ;

Exp 
    :   IfOrLetExp 
        {
            $$ = $1;
        }
    |   TupleExp 
        {
            $$ = $1;
        }
    ;

IfOrLetExp
    :   IF Exp THEN Exp ELSE Exp 
        {
            $$ = createNode(NODE_IFELSE);
            $$->sibling = $2;
            $$->left = $4;
            $$->right = $6;
        }
    |   LET ExpBinds ';' IN Exp 
        {
            $$ = createNode(NODE_LET);
            $$->left = $2;
            $$->right = $5;
        }
    |   LET ExpBinds IN Exp
        {
            $$ = createNode(NODE_LET);
            $$->left = $2;
            $$->right = $4;
        }
    ;

ExpBinds
    :   ExpBind 
        {
            $$ = $1;
        }
    |   ExpBinds ';' ExpBind 
        { 
            $$ = createNode(NODE_EXP_BINDS);
            $$->left = $1;
            $$->right = $3;
        }
    ;

ExpBind
    :   Exp '=' Exp
        {
            $$ = createNode(NODE_ASSIGN);
            $$->left = $1;
            $$->right = $3;
        }
    ;

TupleExp
    :   OrExp 
        {
            $$ = $1;
        }
    |   OrExp ',' TupleRest 
        {
            $$ = createNode(NODE_TUPLE);
            $$->left = $1;
            $$->right = $3;
        }
    ;


TupleRest
    :   TupleExp
        {
            $$ = $1;
        }
    |   IfOrLetExp
        {
            $$ = $1;
        }
    ;

OrExp
    :   OrExp OrOp AndExp 
        {
            $2->left = $1;
            $2->right = $3;
            $$ = $2;
        }
    |   AndExp 
        {
            $$ = $1;
        }
    ;

OrOp
    :   OR  
        { 
            $$ = createNode(NODE_OR); 
        }
    |   NOR 
        { 
            $$ = createNode(NODE_NOR); 
        }
    |   XOR 
        { 
            $$ = createNode(NODE_XOR);  
        } 
    ;

AndExp
    :   RelExp 
        {
            $$ = $1;
        }
    |   AndExp AndOp RelExp 
        {
            $2->left = $1;
            $2->right = $3;
            $$ = $2;
        }
    ;

AndOp
    :   AND  
        {
            $$ = createNode(NODE_AND);
        }
    |   NAND 
        {
            $$ = createNode(NODE_NAND);
        }
    ;

RelExp
    :   AddExp 
        {
            $$ = $1;
        }
    |   RelExp RelOp AddExp
        {
            $2->left = $1;
            $2->right = $3;
            $$ = $2;
        }
    ;

RelOp
    :   EQ 
        {
            $$ = createNode(NODE_EQ); 
        }
    |   NE  
        {
            $$ = createNode(NODE_NE);
        }
    |   '<' 
        {
            $$ = createNode(NODE_LT);
        }
    |   '>' 
        {
            $$ = createNode(NODE_GT);
        }
    |   LE  
        {
            $$ = createNode(NODE_LE);
        }
    |   GE  
        {
            $$ = createNode(NODE_GE);
        }
    ;

AddExp
    :   MulExp 
        {
            $$ = $1;
        }
    |   AddExp AddOp MulExp 
        {
            $2->left = $1;
            $2->right = $3;
            $$ = $2;
        }
    ;

AddOp
    :   '+'     
        {
            $$ = createNode(NODE_ADD);
        }
    |   '-'     
        {
            $$ = createNode(NODE_SUB);
        }
    |   PP      
        {
            $$ = createNode(NODE_PP);
        }
    |   LARROW  
        {
            $$ = createNode(NODE_LARROW);
        }
    ;

MulExp
    :   ExpExp 
        {
            $$ = $1;
        }
    |   MulExp MulOp ExpExp 
        {
            $2->left = $1;
            $2->right = $3;
            $$ = $2;
        }
    ;

MulOp 
    :   '*'    
        {
            $$ = createNode(NODE_MUL);
        } 
    |   '/'    
        {
            $$ = createNode(NODE_DIV);
        }
    |   RARROW 
        {
            $$ = createNode(NODE_RARROW);
        }  
    ;

ExpExp 
    :   UnExp 
        { 
            $$ = $1;
        }
    |   ExpExp '^' UnExp 
        {
            $$ = createNode(NODE_UPT);
            $$->left = $1;
            $$->right = $3;
        }
    ;

UnExp 
    :   SubscriptExp 
        {
            $$ = $1;
        }
    |   UnOp UnExp   
        {
            $1->left = $2;
            $$ = $1;
        }
    ;

UnOp
    :   '#'   
        {   
            $$ = createNode(NODE_SHARP);
        }
    |   '@'   
        {
            $$ = createNode(NODE_AT);
        }
    |   '-'   
        {
            $$ = createNode(NODE_UMINUS);
        } 
    ;

SubscriptExp
    :   AtomicExp 
        {
            $$ = $1;
        }
    |   AtomicExp '[' Exp ']'
        {
            $$ = createNode(NODE_SEQ_REF);
            $$->left = $1;
            $$->right = $3;
        }
    ;

AtomicExp
    :   Const 
        {
            $$ = $1;
        }
    |   SpecialId '(' Exp ')' 
        {
            // TODO:
        }
    |   '{' ApplyBody '}' 
        {
            $$ = $2;
        }
    |   '{' ApplyBody '|' Exp '}' 
        {
            $$ = createNode(NODE_APPLY_TO_EACH);
            $$->left = $2;
            $$->right = $4;
        }
    |   '[' ']' TypeExp 
        {
            $$ = createNode(NODE_EMPTY_SEQ);
            $$->left = $3;
        }
    |   '[' Exp SequenceTail ']'
        {
            $$ = createNode(NODE_SEQ);
            $$->left = $2;
            $$->right = $3;
        }
    |   '(' Exp ')' 
        {
            $$ = $2;
        }
    |   ID 
        {
            $$ = createNode(NODE_ID);
            $$->str_val = $1;
        }
    |   id '(' Exp ')'
        {
            $$ = createNode(NODE_FUNC_CALL);
            $$->left = $1;
            $$->right = $3;
        }
    ;

SpecialId
    :   ANY 
        {
            // TODO:
        }
    ;

ApplyBody
    :   Exp ':' RBinds
        {
            $$ = createNode(NODE_APPLY_BODY);
            $$->left = $1;
            $$->right = $3;
        }
    |   RBinds 
        {
            // TODO:
        }
    ;

RBinds
    :   RBinds ';' RBind
        {
            $$ = createNode(NODE_RBINDS);
            $$->left = $1;
            $$->right = $3;
        }
    |   RBind 
        {
            $$ = $1;
        }
    ;
    
RBind
    :   ID 
        {
            $$ = createNode(NODE_ID);
            $$->str_val = $1;
        }
    |   Exp IN Exp
        {
            $$ = createNode(NODE_IN);
            $$->left = $1;
            $$->right = $3;
        }
    ;

id 
    :   ID
        {
            $$ = createNode(NODE_ID);
            $$->str_val = $1;
        } 
    |   INT   
        {
            $$ = createNode(NODE_TYPE_INT);
        }
    |   FLOAT 
        {
            $$ = createNode(NODE_TYPE_FLOAT);
        }
    |   BOOL  
        {
            $$ = createNode(NODE_TYPE_BOOL);
        }
    |   CHAR  
        {
            $$ = createNode(NODE_TYPE_CHAR);
        }
    ;

SequenceTail
    :   ':' Exp 
        {
            $$ = createNode(NODE_SEQ_TAIL);
            $$->left = $2;
        }
    |   ':' Exp ':' Exp 
        {
            $$ = createNode(NODE_SEQ_TAIL);
            $$->left = $2;
            $$->right = $4;
        }
    |   {
            $$ = createNode(NODE_SEQ_TAIL);
        } 
    ;

Const
    :   INT_CONST 
        {
            $$ = createNode(NODE_CONST_INT);
            $$->int_val = $1; 
        }
    |   FLOAT_CONST
        {
            $$ = createNode(NODE_CONST_FLOAT);
            $$->float_val = $1;
        }
    |   TRUE_CONST
        {
            $$ = createNode(NODE_CONST_TRUE);
        }
    |   FALSE_CONST
        {
            $$ = createNode(NODE_CONST_FALSE);
        }
    |   STRING_CONST
        {
            $$ = createNode(NODE_CONST_STRING);
            $$->str_val = $1;
        }
    ;

%%

int yyerror(const char *s) {
    printf("Syntax error: %s \n",s);

    exit(0);
}
