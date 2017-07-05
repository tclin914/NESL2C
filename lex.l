%{
#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include "y.tab.h"
#define token(s) { /*printf("%s\ttoken%d\n",yytext, s);*/ return(s);}

int line_no = 1;
struct nodeType* newTokenNode(int tokenType);

%}

%option caseless
%x COMMENT

%%



[ \n]*
"%"[^%]*"%" 

"$"    { return '$';}   
"#"    { return '#';}
"@"    { return '@';}
"^"    { return '^';}
"|"    { return '|';}
">"    { return '>';}
"<"    { return '<';}
","    { return ',';}
"("    { return '(';}
")"    { return ')';}
"+"    { return '+';}
"-"    { return '-';}
"*"    { return '*';}
":"    { return ':';}
"/"    { return '/';}
"["    { return '[';}
"]"    { return ']';}
"{"    { return '{';}
"}"    { return '}';}
";"    { return ';';}
"="    { return '=';}
"++"   { return PP;}
"->"   { return RARROW;}
"<-"   { return LARROW;}
"=="   { return EQ;}
"/="   { return NE;}
"<="   { return LE;}
">="   { return GE;}

"FUNCTION" {
    return FUNCTION;
}
"DATATYPE" {
    return DATATYPE;
}

"NUMBER"      { return NUMBER;}
"ORDINAL"   { return ORDINAL;}
"LOGICAL"   { return LOGICAL;}
"ANY"       { return ANY;}
"INT"       {
                yylval.node = newTokenNode(TOKE_INT);
                yylval.node->string = (char*)malloc(yyleng+1);
                strcpy(yylval.node->string, yytext);
                token(ID);
            }
"BOOL"        {
        yylval.node = newTokenNode(TOKE_BOOL); 
        yylval.node->string = (char*)malloc(yyleng+1);
        strcpy(yylval.node->string, yytext);
        token(BOOL);
        
        }
"FLOAT"         {
        yylval.node = newTokenNode(TOKE_FLOAT); /*token(FLOAT);*/ 
        yylval.node->string = (char*)malloc(yyleng+1);
        strcpy(yylval.node->string, yytext);
        token(ID);
        
/*"TIME"            {yylval.node = newTokenNode(TIME); token(TIME);} */
        }
"CHAR"  { return TOKE_CHAR;} 
"IN"    { return IN;   }
"IF"    { return IF;   }
"ELSE"  { return ELSE; }
"THEN"  { return THEN; }
"LET"   { return LET;  }
"AND"   { return AND;  }
"NAND"  { return NAND; }
"NOR"   { return NOR;  }
"XOR"   { return XOR;  }
"OR"    { return OR;   }
"PI"    {
         yylval.rvalue = 3.141592653589793;
         return floatconst;
        }

[0-9]+ { 
    yylval.ivalue = atoi(yytext);
    return intconst;
}
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval.rvalue = atof(yytext);
    return floatconst;
}
"T" {       /* not decided whether use ivalue or string to present boolean */
            yylval.ivalue = 1;
            yylval.string = (char*)malloc(yyleng+1);
            strcpy(yylval.string, yytext);
            return boolconst;
}
"F" {
            yylval.ivalue = 0;
            yylval.string = (char*)malloc(yyleng+1);
            strcpy(yylval.string, yytext);
            return boolconst;
}
[_a-z0-9]+  {
            yylval.string = (char*)malloc(yyleng+1);
            strcpy(yylval.string, yytext);
            return ID;
}

\"[^"]*\"   {
            yylval.string = (char*)malloc(yyleng+1);
            strcpy(yylval.string, yytext);
            return stringconst;
}

"//".*"\n"  {                           //string value
    printf("%d: %s", line_no++, yytext);
}

%%
struct nodeType* newTokenNode(int tokenType) {
    struct nodeType *node = newNode(NODE_TOKEN);
    node->token = tokenType;

    return node;
}
int yywrap(){
    
    return 1;
}

