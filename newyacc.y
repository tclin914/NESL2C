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
%token <node> '{' '}' '(' ')' ';' '=' ',' '[' ']' ':' '|' '$'


%type <node> goal TopLevel FunId EndMark FunTypeDef TypeExp PairTypes TypeList
%type <node> Exp IfOrLetExp ExpBinds ExpBind TupleExp TupleRest OrExp OrOp 
%type <node> AndExp AndOp RelExp RelOp AddExp AddOp MulExp MulOp ExpExp UnExp 
%type <node> UnOp SubscriptExp AtomicExp SpecialId ApplyBody RBinds RBind
%type <node> SequenceTail Const 

%type <node> '<' '>' '+' '-' PP 

%type <node> '*' '/' '^' '#' '@' 


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

goal: TopLevel {ASTRoot=newNode(NODE_LIST);addChild(ASTRoot,$1);$$=ASTRoot; }
    | goal TopLevel {$$=$1; addChild($$,$2);}
    ;

TopLevel 
        : FUNCTION FunId Exp ':' FunTypeDef '=' Exp EndMark
        {
            $$=$1;
            $$->nodeType = NODE_FUNC;
            $1->string =  (char*)malloc(sizeof(strlen($2->string)));
            strcpy($$->string, $2->string);
            $6->nodeType = NODE_OP;
            $6->op = OP_BIND;
        }
        | FUNCTION FunId Exp '=' Exp EndMark
        | DATATYPE ID '(' TypeList ')' EndMark
        | Exp '=' Exp EndMark
        | Exp EndMark
        ;

FunId   : ID{
                $$=$1;
        }
        | SpecialId{
                $$=$1;
        }
        ;

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
    | LET ExpBinds IN Exp
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
    | OrExp ',' TupleRest
    ;


TupleRest
    : TupleExp
    | IfOrLetExp
    ;

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
    : '#' | '@' | '-'
    ;

SubscriptExp
    : AtomicExp
    | AtomicExp '[' Exp ']'
    ;

AtomicExp
    : Const
    | SpecialId '(' Exp ')'
/*  TODO extract TIME(EXP) FLOAT(EXP) */
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
/*    TODO pattern IN Exp
*/
    ;

SequenceTail
    : ':' Exp
    | ':' Exp ':' Exp
    | {$$ = newNode(NODE_EMPTY);} 
    ;

Const
    : intconst 
    | floatconst
    | boolconst
    | stringconst
    ;

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
