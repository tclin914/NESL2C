%{
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
%}

%token FUNCTION NUMBER ORDINAL LOGICAL ANY INT BOOL FLOAT CHAR 
%token IF THEN ELSE LET IN OR NOR XOR AND NAND 
%token intconst floatconst name boolconst stringconst
%token '{' '}' '(' ')' ';' '=' ',' '[' ']'

%start goal

%%

goal : toplevel { printf("toplevel\n"); }
    ;

toplevel : 
        FUNCTION name pattern ':' typedef '=' exp ';' |
        FUNCTION name pattern 
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

exp : name 
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
