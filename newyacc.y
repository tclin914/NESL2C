%{
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "newnode.h"
#include "symtab.h"

int yydebug =1;

struct nodeType * newOpNode(int op); 
extern struct nodeType* ASTRoot;
%}

%union {
    struct nodeType *node;
    int number ;
    char * string;
}

%token <node> FUNCTION DATATYPE NUMBER ORDINAL LOGICAL ANY INT BOOL FLOAT CHAR 
%token <node> IF THEN ELSE LET IN OR NOR XOR AND NAND RARROW LARROW NE EQ LE GE
%token <node> intconst floatconst ID boolconst stringconst TIME
%token <node> '{' '}' '(' ')' ';' '=' ',' '[' ']' ':' '|'

/*
%type <node> goal nesl TopLevel FunId TypecaseRule TypecaseLHS EndMark
%type <node> FunTypeDef TypeExp PairTypes TypeList Exp IfOrLetExp ExpBind
%type <node> TupleExp TupleRest OrExp AndExp AndOp RelExp RelOp AddExp AddOp
%type <node> MulExp ExpExp UnOp SubscriptExp ApplyExp AtomicExp SpecialId
%type <node> ApplyBody RBinds RBind SequenceTail Const Pattern AtomicPat
*/

%left ','
%left OR NOR XOR
%left AND NAND
%left EQ NE '<' '>' LE GE
%left '+' '-' PP LARROW
%left '*' '/' RARROW
%left '^' 
%left '#' '@' 
%nonassoc UMINUS

%start goal

%%

goal: TopLevel
    | goal TopLevel
    ;

TopLevel 
        : FUNCTION FunId Exp ':' FunTypeDef '=' Exp EndMark
        | Exp '=' Exp EndMark
        | Exp EndMark
        ;

FunId   : ID
        | SpecialId
        ;
/*
TypecaseRule
        : '|' TypecaseLHS ':' Exp  
        ;
TypecaseLHS 
        : ID
        | FUNCTION
        ;
*/

EndMark : ';' 
        | '$'
        ;

FunTypeDef : TypeExp RARROW TypeExp
        ;

TypeExp : ID
        | ID '(' TypeList ')'
        | '[' TypeExp ']'
        | '('PairTypes ')'
        ;

PairTypes : PairTypes ',' TypeExp
        | TypeExp 
        ;

TypeList : TypeList ',' TypeExp 
        | TypeExp
        ;


Exp : IfOrLetExp
    | TupleExp
    ;

IfOrLetExp
    : IF Exp THEN Exp ELSE Exp
    | LET ExpBinds ';' IN Exp
    ;

ExpBinds
    : ExpBind
    | ExpBinds ';' ExpBind 
    ;

ExpBind
    : Exp '=' Exp
    ;

TupleExp
    : OrExp 
    | Exp ',' OrExp
    ;

/*
TupleRest
    : TupleExp
    | IfOrLetExp
    ;
*/

OrExp
    : OrExp OrOp AndExp
    | AndExp
    ;

OrOp: OR | NOR | XOR ;

AndExp
    : RelExp
    | AndExp AndOp RelExp
    ;

AndOp
    : AND
    | NAND
    ;

RelExp
    : AddExp
    | RelExp RelOp AddExp
    ;

RelOp
    :  EQ | NE
    | '<' | '>'
    | LE | GE
    ;

AddExp
    : MulExp
    | AddExp AddOp MulExp
    ;

AddOp
    : '+' | '-' | PP | LARROW
    ;

MulExp
    : ExpExp
    | MulExp MulOp ExpExp
    ;

MulOp
    : '*' | '/' | RARROW  
    ;

ExpExp 
    : UnExp 
    | ExpExp '^' UnExp
    ;

UnExp 
    : SubscriptExp
    | UnOp UnExp
    ;

UnOp
    : '#' | '@' | UMINUS
    ;

SubscriptExp
    : AtomicExp
    | AtomicExp '[' Exp ']'
/*    | newpattern
    | newpattern '[' Exp ']' 
*/
    ;
/*
ApplyExp
    : AtomicExp
    | AtomicExp AtomicExp
    ;
*/
AtomicExp
    : Const
    | SpecialId '(' Exp ')'
/*    | TIME '(' Exp ')' */
    | '{' ApplyBody '}'
    | '{' ApplyBody '|' Exp '}'
    | '[' ']' TypeExp
    | '[' Exp SequenceTail ']'
    | '(' Exp ')'
    | ID
    | ID '(' Exp ')'
    
    ;


SpecialId
    : ANY
    ;

ApplyBody
    : Exp ':' RBinds
    | RBinds
    ;

RBinds
    : RBinds ';' RBind
    | RBind
    ;
    
RBind
    : ID
    | Exp IN Exp
/*    | pattern IN Exp
*/
    ;

SequenceTail
    : ':' Exp
    | ':' Exp ':' Exp
    | 
    ;

Const
    : intconst 
    | floatconst
    | boolconst
    | stringconst
    ;
/*
newpattern :
    ID
    | ID '(' Exp ')'
    | '(' Exp ')'
    | newpattern ',' ID
    ;
pattern
    : AtomicPat
    | pattern ',' AtomicPat
    ;

AtomicPat
    : ID
    | ID '(' pattern ')'
    | '(' pattern ')'
    ;
*/

%%

struct nodeType *ASTRoot;
struct nodeType * newOpNode(int op) {
    struct nodeType *node = newNode(NODE_OP);
    node -> op = op;
    return node;
}

int yyerror(const char *s) {
    printf("Syntax error\n");
    //printf("errror: %s",s);

    exit(0);
}

int main(int argc, char** argv){
    printf("%d: %s\n",argc,argv[0]);
    
    yyparse();
    printf("************************\n");
    printf("*** NO PARSING ERROR ***\n");
    printf("************************\n");
    
    printTree(ASTRoot, 0);
    
    semanticCheck(ASTRoot);
    printf("************************\n");
    printf("** NO SEMANTIC ERROR ***\n");
    printf("************************\n");

    FILE* fptr;
    fptr = fopen("output/NESL2C_test.c","w");
    if(!fptr){
        printf("failed create output file! exit\n");
        exit(1);
    }
    // fprintf(fptr, "This is a test\n");
    fprintf(fptr, "#include<stdio.h>\n#include<stdlib.h>\n\nint main(){\n\t");

    codegen(fptr, ASTRoot);

    fprintf(fptr, "\n}");
    printf("************************\n");
    printf("***** CODEGEN DONE *****\n");
    printf("************************\n");
    if(fptr==NULL)
        fclose(fptr);
    
    return 0;
}
