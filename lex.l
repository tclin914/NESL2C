%{
#include <stdio.h>
#include "y.tab.h"
#define token(s) {printf("%s\ttoken%d\n",yytext, s);return(s);}

%}

A [aA]
B [bB]
C [cC]
D [dD]
E [eE]
F [fF]
G [gG]
H [hH]
I [iI]
J [jJ]
K [kK]
L [lL]
M [mM]
N [nN]
O [oO]
P [pP]
Q [qQ]
R [rR]
S [sS]
T [tT]
U [uU]
V [vV]
W [wW]
X [xX]
Y [yY]
Z [zZ]
NQUOTE [^']


%%

[ \n]* {
    /* Just to eat the spaces and new lines
           so that they won't appear in output */
}
, {
            token(',');
}
"(" {
            token('(');
}
")" {
            token(')');
}
"+" {
            token('+');
}
"-" {
            token('-');
}
"*" {
            token('*');
}
: {
            token(':');
}
"/" {
            token('/');
} 
"[" {
            token('[');
}
"]" {
            token(']');
}
"{" {
            token('{');
}
"}" {
            token('}');
}

; {
            token(';');
}
"=" {
            token('=');
}
{F}{U}{N}{C}{T}{I}{O}{N} {
    token(FUNCTION);    
}
{D}{A}{T}{A}{T}{Y}{P}{E} {
    token(DATATYPE);
}

{N}{U}{M}{B}{E}{R}      {token(NUMBER);  }
{O}{R}{D}{I}{N}{A}{L}   {token(ORDINAL); }
{L}{O}{G}{I}{C}{A}{L}   {token(LOGICAL); }
{A}{N}{Y}               {token(ANY);}
{I}{N}{T}               {token(INT);}
{B}{O}{O}{L}            {token(BOOL);}
{F}{L}{O}{A}{T}         {token(FLOAT);}
{C}{H}{A}{R}            {token(CHAR);}
{I}{N}                  {token(IN);}
{I}{F}                  {token(IF);}
{E}{L}{S}{E}            {token(ELSE);}
{T}{H}{E}{N}            {token(THEN);}


[-+]?[0-9]+ { token(intconst); }
[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    //return(floatconst);
    token(floatconst);
}
[_a-zA-Z0-9]+  {
            token(name);
}
[{T}{F}]        {
            token(boolconst);
}
\"[^"]*\"     {
            token(stringconst);
}

%%
yywrap(){
    return(1);
    }

