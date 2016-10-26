%{
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
int yydebug =1;
%}

%token FUNCTION DATATYPE NUMBER ORDINAL LOGICAL ANY INT BOOL FLOAT CHAR 
%token IF THEN ELSE LET IN OR NOR XOR AND NAND 
%token intconst floatconst name boolconst stringconst
%token '{' '}' '(' ')' ';' '=' ',' '[' ']'

%start goal

%%

goal : sequence ;

nesl : toplevel { printf("toplevel\n"); }
        | 
        ;

toplevel : 
        FUNCTION name pattern funcopt |
        DATATYPE name typedef ';' |
        pattern '=' exp |
        exp 
        ;

funcopt : ':' typedef '=' exp ';' 
        |
        ;

pattern : name |
        name '(' pattern ')' |
        pattern ',' pattern |
        '(' pattern ')' 
        ;

typedef : typeexp |
        typeexp ':'':' '(' typebinds ')'
        ;

typebinds : typebind |
        typebind ';' typebinds
        ;

typebind : name IN typeclass 
        ;

typeexp : basetype |
        name |
        typeexp '-''>' typeexp |
        typeexp ',' typeexp |
        name '(' typelist ')' |
        '[' typeexp ']' |
        '(' typeexp ')'
        ;

typelist : typeexp |
           typeexp ',' typelist 
         ;

typeclass : NUMBER | ORDINAL | LOGICAL | ANY 
        ;

basetype : INT | BOOL | FLOAT | CHAR 
        ;

exp : const 
    | name 
    | IF exp THEN exp ELSE exp 
    | LET expbinds IN exp 
    | '{' expoption1 rbinds expoption2 '}' 
    | exp exp
    | exp binop exp
    | unaryop exp
    | sequence
    | exp '[' exp ']'
    | '(' exp ')'
    ;

expoption1 : exp ':' 
    | 
    ;

expoption2 : '|' exp
    | 
    ;

expbinds :
    pattern '=' exp expbindsoption
    ;

expbindsoption : ';' expbinds 
    | 
    ;

pattern : name 
    | name '(' pattern ')'
    | pattern ',' pattern 
    | '(' pattern ')'
    ;

rbinds : rbind rbinds
    | ';' rbind
    |
    ;

rbind : pattern IN exp 
    | name 
    ;

sequence : '[' explist ']'
    | '[' ']' typeexp
    | '[' exp ':' exp seqoption ']'
    ;
seqoption : ':' exp
    ;

explist : exp explist 
    | ',' explist
    |
    ;

const : intconst 
    |   floatconst 
    |   boolconst 
    |   stringconst
    ;

binop : ','
    | OR | NOR | XOR
    | AND | NAND
    | "==" | "/=" | '<' | '>' | "<=" | ">="
    | '+'  | '-' | "++" | "<-"
    | '*'  | '/' | "->" 
    | '^'
    ;

unaryop :   '#'
    |   '@'
    |   '-'
    ;



%%

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
