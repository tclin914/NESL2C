%{
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "node.h"

int yydebug =1;

struct nodeType * nodeOpNode(int op);
%}

%union {
    struct nodeType *node;
    int number ;
    char * string;
}

%token <node> FUNCTION DATATYPE NUMBER ORDINAL LOGICAL ANY INT BOOL FLOAT CHAR 
%token <node> IF THEN ELSE LET IN OR NOR XOR AND NAND 
%token <node> intconst floatconst nametoken boolconst stringconst
%token <node> '{' '}' '(' ')' ';' '=' ',' '[' ']' ':' '+' '-' '*' '/'

%type <node> goal nesl toplevel funcopt typedef typebinds typebind
%type <node> typeexp typelist typeclass basetype exp expoption1
%type <node> expoption2 expbinds expbindsoption pattern rbinds
%type <node> rbind sequence seqoption explist const binop unaryop

%start goal

%%

goal : toplevel  {$$=newNode(NODE_ERROR);}
    ;

nesl : toplevel { printf("toplevel\n"); $$=newNode(NODE_ERROR);}
        | {$$=newNode(NODE_ERROR);}
        ;

toplevel : 
        FUNCTION nametoken pattern funcopt {$$=newNode(NODE_ERROR);}|
        DATATYPE nametoken typedef ';' {$$=newNode(NODE_ERROR);}|
        pattern '=' exp {$$=newNode(NODE_ERROR);}|
        exp {$$=newNode(NODE_ERROR);}
        ;

funcopt : ':' typedef '=' exp ';' {$$=newNode(NODE_ERROR);}
        |{$$=newNode(NODE_ERROR);}
        ;

pattern : nametoken {$$=newNode(NODE_ERROR);}|
        nametoken '(' pattern ')' {$$=newNode(NODE_ERROR);}|
        pattern ',' pattern {$$=newNode(NODE_ERROR);}|
        '(' pattern ')' {$$=newNode(NODE_ERROR);}
        ;

typedef : typeexp {$$=newNode(NODE_ERROR);}|
        typeexp ':'':' '(' typebinds ')'{$$=newNode(NODE_ERROR);}
        ;

typebinds : typebind {$$=newNode(NODE_ERROR);}|
        typebind ';' typebinds{$$=newNode(NODE_ERROR);}
        ;

typebind : nametoken IN typeclass {$$=newNode(NODE_ERROR);}
        ;

typeexp : basetype {$$=newNode(NODE_ERROR);}|
        nametoken {$$=newNode(NODE_ERROR);}|
        typeexp '-''>' typeexp {$$=newNode(NODE_ERROR);}|
        typeexp ',' typeexp {$$=newNode(NODE_ERROR);}|
        nametoken '(' typelist ')' {$$=newNode(NODE_ERROR);}|
        '[' typeexp ']' {$$=newNode(NODE_ERROR);}|
        '(' typeexp ')'{$$=newNode(NODE_ERROR);}
        ;

typelist : typeexp {$$=newNode(NODE_ERROR);} |
           typeexp ',' typelist {$$=newNode(NODE_ERROR);}
         ;

typeclass : NUMBER {$$=newNode(NODE_ERROR);}
        | ORDINAL {$$=newNode(NODE_ERROR);}
        | LOGICAL {$$=newNode(NODE_ERROR);}
        | ANY {$$=newNode(NODE_ERROR);}
        ;

basetype : INT {$$=newNode(NODE_ERROR);}
        | BOOL {$$=newNode(NODE_ERROR);}
        | FLOAT {$$=newNode(NODE_ERROR);}
        | CHAR {$$=newNode(NODE_ERROR);}
        ;

exp : const {$$=newNode(NODE_ERROR);}
    | nametoken {$$=newNode(NODE_ERROR);}
    | IF exp THEN exp ELSE exp {$$=newNode(NODE_ERROR);}
    | LET expbinds IN exp {$$=newNode(NODE_ERROR);}
    | '{' expoption1 rbinds expoption2 '}' {$$=newNode(NODE_ERROR);}
    | exp exp{$$=newNode(NODE_ERROR);}
    | exp binop exp{$$=newNode(NODE_ERROR);}
    | unaryop exp{$$=newNode(NODE_ERROR);}
    | sequence{$$=newNode(NODE_ERROR);}
    | exp '[' exp ']'{$$=newNode(NODE_ERROR);}
    | '(' exp ')'{$$=newNode(NODE_ERROR);}
    ;

expoption1 : exp ':' {$$=newNode(NODE_ERROR);}
    | {$$=newNode(NODE_ERROR);}
    ;

expoption2 : '|' exp{$$=newNode(NODE_ERROR);}
    | {$$=newNode(NODE_ERROR);}
    ;

expbinds :
    pattern '=' exp expbindsoption{$$=newNode(NODE_ERROR);}
    ;

expbindsoption : ';' expbinds {$$=newNode(NODE_ERROR);}
    | {$$=newNode(NODE_ERROR);}
    ;

rbinds : rbind rbinds{$$=newNode(NODE_ERROR);}
    | ';' rbind{$$=newNode(NODE_ERROR);}
    |{$$=newNode(NODE_ERROR);}
    ;

rbind : pattern IN exp {$$=newNode(NODE_ERROR);}
    | nametoken {$$=newNode(NODE_ERROR);}
    ;

sequence : '[' explist ']'{$$=newNode(NODE_ERROR);}
    | '[' ']' typeexp{$$=newNode(NODE_ERROR);}
    | '[' exp ':' exp seqoption ']'{$$=newNode(NODE_ERROR);}
    ;
seqoption : ':' exp{$$=newNode(NODE_ERROR);}
    ;

explist : exp explist {$$=newNode(NODE_ERROR);}
    | ',' explist{$$=newNode(NODE_ERROR);}
    |{$$=newNode(NODE_ERROR);}
    ;

const : intconst {$$=newNode(NODE_ERROR);}
    |   floatconst {$$=newNode(NODE_ERROR);}
    |   boolconst {$$=newNode(NODE_ERROR);}
    |   stringconst{$$=newNode(NODE_ERROR);}
    ;

binop : ','{$$=newNode(NODE_ERROR);}
    | OR {$$=newNode(NODE_ERROR);}| NOR {$$=newNode(NODE_ERROR);}| XOR{$$=newNode(NODE_ERROR);}
    | AND {$$=newNode(NODE_ERROR);}| NAND{$$=newNode(NODE_ERROR);}
    | "==" {$$=newNode(NODE_ERROR);}| "/=" {$$=newNode(NODE_ERROR);}| '<' {$$=newNode(NODE_ERROR);}| '>' {$$=newNode(NODE_ERROR);}| "<=" {$$=newNode(NODE_ERROR);}| ">="{$$=newNode(NODE_ERROR);}
    | '+' {$$=newNode(NODE_ERROR);} | '-' {$$=newNode(NODE_ERROR);}| "++" {$$=newNode(NODE_ERROR);}| "<-"{$$=newNode(NODE_ERROR);}
    | '*' {$$=newNode(NODE_ERROR);} | '/' {$$=newNode(NODE_ERROR);}| "->"{$$=newNode(NODE_ERROR);}
    | '^' {$$=newNode(NODE_ERROR);}
    ;

unaryop :   '#' {$$=newNode(NODE_ERROR);}
    |   '@'  {$$=newNode(NODE_ERROR);}
    |   '-' {$$=newNode(NODE_ERROR);}
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

int main(){
    yyparse();
    printf("parse done\n");
 return 0;
}
