%{
#include <stdio.h>

%}

%token FUNCTION NUMBER ORDINAL LOGICAL ANY INT BOOL FLOAT CHAR 
%token IF THEN ELSE LET IN OR NOR XOR AND NAND 
%token intconst floatconst name boolconst stringconst
/*%token toplevel name pattern typedef exp typebinds typebind typeexp
%token typelist typeclass basetype const rbinds rbind sequence explist const 
%token binop unaryop 
%token goal toplevel */
/* left 
%left NOT
%left GE LT LE GT EQ NE
%left PLUS MINUS
%left STAR SLASH
%nonassoc UMINUS                 // uni minus
*/
%start goal

%%

goal : toplevel{
    printf("toplevel\n");
}

toplevel : FUNCTION name pattern ':' typedef '=' exp ';' |
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

typebind : name IN typeclass ;

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

typeclass : NUMBER | ORDINAL | LOGICAL | ANY ;

basetype : INT | BOOL | FLOAT | CHAR ;

exp : name;

%%
