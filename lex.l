%{
#include <stdio.h>
#include "node.h"
#include "y.tab.h"
#define token(s) {printf("%s\ttoken%d\n",yytext, s);return(s);}

int line_no = 1;
struct nodeType* newTokenNode(int tokenType);

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

">" {
            token('>');
}
"<" {
            token('<');
}
"," {
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
":" {
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

";" {
            token(';');
}
"=" {
            token('=');
}
{F}{U}{N}{C}{T}{I}{O}{N} {
    yylval.node = newTokenNode(FUNCTION);
    token(FUNCTION);    
}
{D}{A}{T}{A}{T}{Y}{P}{E} {
    yylval.node = newTokenNode(DATATYPE);
    token(DATATYPE);
}

{N}{U}{M}{B}{E}{R}      {yylval.node = newTokenNode(NUMBER); token(NUMBER);  }
{O}{R}{D}{I}{N}{A}{L}   {yylval.node = newTokenNode(ORDINAL); token(ORDINAL); }
{L}{O}{G}{I}{C}{A}{L}   {yylval.node = newTokenNode(LOGICAL); token(LOGICAL); }
{A}{N}{Y}               {yylval.node = newTokenNode(ANY); token(ANY);}
{I}{N}{T}               {yylval.node = newTokenNode(INT); token(INT);}
{B}{O}{O}{L}            {yylval.node = newTokenNode(BOOL); token(BOOL);}
{F}{L}{O}{A}{T}         {yylval.node = newTokenNode(FLOAT); /*token(FLOAT);*/ token(nametoken);}
{C}{H}{A}{R}            {yylval.node = newTokenNode(CHAR); token(CHAR);}
{I}{N}                  {yylval.node = newTokenNode(IN); token(IN);}
{I}{F}                  {yylval.node = newTokenNode(IF); token(IF);}
{E}{L}{S}{E}            {yylval.node = newTokenNode(ELSE); token(ELSE);}
{T}{H}{E}{N}            {yylval.node = newTokenNode(THEN); token(THEN);}
{L}{E}{T}               {yylval.node = newTokenNode(LET); token(LET);}


[-+]?[0-9]+ { 
    yylval.node = newTokenNode(intconst);
    yylval.node->valueValid = VALUE_I_VALID;
    yylval.node->iValue = atoi(yytext);
    token(intconst); 
}
[-+]?[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval.node = newTokenNode(floatconst);
    yylval.node->valueValid = VALUE_R_VALID;
    yylval.node->rValue = atof(yytext);
    token(floatconst);
}
[_a-zA-Z0-9]+  {
            yylval.node = newTokenNode(nametoken);
            yylval.node->string = (char*)malloc(yyleng+1);
            strcpy(yylval.node->string, yytext);
            token(nametoken);
}
{T}        {
            token(boolconst);
}
{F} {
            token(boolconst);
}
\"[^"]*\"     {
            token(stringconst);
}

"//".*"\n"  {                           //string value
    printf("%d: %s", line_no++, yytext);
}

%%
struct nodeType* newTokenNode(int tokenType) {
    struct nodeType *node = newNode(NODE_TOKEN);
    node->tokenType = tokenType;

    return node;
}
yywrap(){
    return(1);
    }

