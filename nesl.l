%{
#include <stdio.h>
#include <stdlib.h>
#include "y.tab.h"

int line_no = 1;

%}

%option caseless
%x COMMENT

%%

[ \n]*
"%"[^%]*"%" 

"$"                                     { return '$';}   
"#"                                     { return '#';}
"@"                                     { return '@';}
"^"                                     { return '^';}
"|"                                     { return '|';}
">"                                     { return '>';}
"<"                                     { return '<';}
","                                     { return ',';}
"("                                     { return '(';}
")"                                     { return ')';}
"+"                                     { return '+';}
"-"                                     { return '-';}
"*"                                     { return '*';}
":"                                     { return ':';}
"/"                                     { return '/';}
"["                                     { return '[';}
"]"                                     { return ']';}
"{"                                     { return '{';}
"}"                                     { return '}';}
";"                                     { return ';';}
"="                                     { return '=';}
"++"                                    { return PP;}
"->"                                    { return RARROW;}
"<-"                                    { return LARROW;}
"=="                                    { return EQ;}
"/="                                    { return NE;}
"<="                                    { return LE;}
">="                                    { return GE;}

"FUNCTION"                              { return FUNCTION;}
"DATATYPE"                              { return DATATYPE;}

"NUMBER"                                { return NUMBER;}
"ORDINAL"                               { return ORDINAL;}
"LOGICAL"                               { return LOGICAL;}
"ANY"                                   { return ANY;}
"IN"                                    { return IN;}
"IF"                                    { return IF;}
"ELSE"                                  { return ELSE;}
"THEN"                                  { return THEN;}
"LET"                                   { return LET;}
"AND"                                   { return AND;}
"NAND"                                  { return NAND;}
"NOR"                                   { return NOR;}
"XOR"                                   { return XOR;}
"OR"                                    { return OR;}
"PI"                                    { return PI;}

[0-9]+                                  { 
                                            yylval.tokenint = atoi(yytext);
                                            return INT_CONST;
                                        }

[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?        {
                                            yylval.tokenfloat = atof(yytext);
                                            return FLOAT_CONST;
                                        }

"T"                                     { return TRUE_CONST;}
"F"                                     { return FALSE_CONST;}

"INT"                                   { return INT;}
"BOOL"                                  { return BOOL;}
"FLOAT"                                 { return FLOAT;}
"CHAR"                                  { return CHAR;} 


[_a-z0-9]+                              {
                                            yylval.tokenstr = strdup(yytext);
                                            return ID;
                                        }

\"[^"]*\"                               {
                                            yylval.tokenstr = strdup(yytext);
                                            return STRING_CONST;
                                        }

"//".*"\n"  {                           //string value
    printf("%d: %s", line_no++, yytext);
}

%%

int yywrap(){
    
    return 1;
}

