%{
#include <stdio.h>
#include <stdlib.h>
#include "newnode.h"
#include "y.tab.h"
#define token(s) {printf("%s\ttoken%d\n",yytext, s);return(s);}

int line_no = 1;
struct nodeType* newTokenNode(int tokenType);

%}

%option caseless
%x COMMENT

%%

[ \n]* {
    /* Just to eat the spaces and new lines
           so that they won't appear in output */
    /*}

TODO comment %...% (low priority)

"%"    { BEGIN COMMENT; }
<COMMENT>"%"       {BEGIN 0;}
<COMMENT>[^*\n]*       
<COMMENT>\n {
             }
*/


}

"%"[^%]*"%" 

"#"     token('#');
"@"     token('@');
"^"     token('^');
"|"     token('|');

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

"++" {      token(PP);
}

"->" {
            token(RARROW);
}

"<-" {
            token(LARROW);
}
"=="    token(EQ);
"/="    token(NE);
"<="    token(LE);
">="    token(GE);



"FUNCTION" {
    yylval.node = newTokenNode(FUNCTION);
    token(FUNCTION);    
}
"DATATYPE" {
    yylval.node = newTokenNode(DATATYPE);
    token(DATATYPE);
}

"OR"      {yylval.node = newTokenNode(OR); token(OR);  }
"NUMBER"      {yylval.node = newTokenNode(NUMBER); token(NUMBER);  }
"ORDINAL"   {yylval.node = newTokenNode(ORDINAL); token(ORDINAL); }
"LOGICAL"   {yylval.node = newTokenNode(LOGICAL); token(LOGICAL); }
"ANY"               {yylval.node = newTokenNode(ANY); token(ANY);}
"INT"       {
                yylval.node = newTokenNode(INT);
                yylval.node->string = (char*)malloc(yyleng+1);
                strcpy(yylval.node->string, yytext);
                token(ID);
            }
"BOOL"        {
        yylval.node = newTokenNode(BOOL); 
        yylval.node->string = (char*)malloc(yyleng+1);
        strcpy(yylval.node->string, yytext);
        token(BOOL);
        
        }
"FLOAT"         {
        yylval.node = newTokenNode(FLOAT); /*token(FLOAT);*/ 
        yylval.node->string = (char*)malloc(yyleng+1);
        strcpy(yylval.node->string, yytext);
        token(ID);
        
        }
"CHAR"            {yylval.node = newTokenNode(CHAR); token(CHAR);}
"IN"                  {yylval.node = newTokenNode(IN); token(IN);}
"IF"                  {yylval.node = newTokenNode(IF); token(IF);}
"ELSE"            {yylval.node = newTokenNode(ELSE); token(ELSE);}
"THEN"            {yylval.node = newTokenNode(THEN); token(THEN);}
"LET"               {yylval.node = newTokenNode(LET); token(LET);}


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
[_a-z0-9]+  {
            yylval.node = newTokenNode(ID);
            yylval.node->string = (char*)malloc(yyleng+1);
            strcpy(yylval.node->string, yytext);
            token(ID);
}
'T'        {
            token(boolconst);
}
'F' {
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

