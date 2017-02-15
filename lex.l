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

"$"    {yylval.node = newTokenNode(NODE_MARK); token('$');     }   
"#"    {yylval.node = newTokenNode(NODE_MARK); token('#');     }
"@"    {yylval.node = newTokenNode(NODE_MARK); token('@');     }
"^"    {yylval.node = newTokenNode(NODE_MARK); token('^');     }
"|"    {yylval.node = newTokenNode(NODE_MARK); token('|');     }
">"    {yylval.node = newTokenNode(NODE_MARK); token('>');     }
"<"    {yylval.node = newTokenNode(NODE_MARK); token('<');     }
","    {yylval.node = newTokenNode(NODE_MARK); token(',');     }
"("    {yylval.node = newTokenNode(NODE_MARK); token('(');     }
")"    {yylval.node = newTokenNode(NODE_MARK); token(')');     }
"+"    {yylval.node = newTokenNode(NODE_MARK); token('+');     }
"-"    {yylval.node = newTokenNode(NODE_MARK); token('-');     }
"*"    {yylval.node = newTokenNode(NODE_MARK); token('*');     }
":"    {yylval.node = newTokenNode(NODE_MARK); token(':');     }
"/"    {yylval.node = newTokenNode(NODE_MARK); token('/');     }
"["    {yylval.node = newTokenNode(NODE_MARK); token('[');     }
"]"    {yylval.node = newTokenNode(NODE_MARK); token(']');     }
"{"    {yylval.node = newTokenNode(NODE_MARK); token('{');     }
"}"    {yylval.node = newTokenNode(NODE_MARK); token('}');     }
";"    {yylval.node = newTokenNode(NODE_MARK); token(';');     }
"="    {yylval.node = newTokenNode(NODE_MARK); token('=');     }
"++"   {yylval.node = newTokenNode(NODE_MARK); token(PP);      }
"->"   {yylval.node = newTokenNode(NODE_MARK); token(RARROW);  }
"<-"   {yylval.node = newTokenNode(NODE_MARK); token(LARROW);  }
"=="   {yylval.node = newTokenNode(NODE_MARK); token(EQ);      }
"/="   {yylval.node = newTokenNode(NODE_MARK); token(NE);      }
"<="   {yylval.node = newTokenNode(NODE_MARK); token(LE);      }
">="   {yylval.node = newTokenNode(NODE_MARK); token(GE);      }

"FUNCTION" {
    yylval.node = newTokenNode(FUNCTION);
    token(FUNCTION);    
}
"DATATYPE" {
    yylval.node = newTokenNode(DATATYPE);
    token(DATATYPE);
}

"NUMBER"      {yylval.node = newTokenNode(NUMBER); token(NUMBER);  }
"ORDINAL"   {yylval.node = newTokenNode(ORDINAL); token(ORDINAL); }
"LOGICAL"   {yylval.node = newTokenNode(LOGICAL); token(LOGICAL); }
"ANY"               {yylval.node = newTokenNode(ANY); token(ANY);}
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
"CHAR"            {yylval.node = newTokenNode(TOKE_CHAR); token(CHAR);}
"IN"                  {yylval.node = newTokenNode(IN); token(IN);}
"IF"                  {yylval.node = newTokenNode(IF); token(IF);}
"ELSE"            {yylval.node = newTokenNode(ELSE); token(ELSE);}
"THEN"            {yylval.node = newTokenNode(THEN); token(THEN);}
"LET"               {yylval.node = newTokenNode(LET); token(LET);}
"AND"               {yylval.node = newTokenNode(AND); token(AND);}
"NAND"               {yylval.node = newTokenNode(NAND); token(NAND);}
"NOR"               {yylval.node = newTokenNode(NOR); token(NOR);}
"XOR"               {yylval.node = newTokenNode(XOR); token(XOR);}
"OR"      {yylval.node = newTokenNode(OR); token(OR);  }


[0-9]+ { 
    yylval.node = newTokenNode(intconst);
    yylval.node->valueValid = VALUE_I_VALID;
    yylval.node->iValue = atoi(yytext);
    token(intconst); 
}
[0-9]*\.[0-9]+([eE][-+]?[0-9]+)? {
    yylval.node = newTokenNode(floatconst);
    yylval.node->valueValid = VALUE_R_VALID;
    yylval.node->rValue = atof(yytext);
    token(floatconst);
}
[_a-z0-9]+  {
            yylval.node = newTokenNode(TOKE_ID);
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
            yylval.node = newTokenNode(TOKE_STRING);
            yylval.node->string = (char*)malloc(yyleng+1);
            strcpy(yylval.node->string, yytext);
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
int yywrap(){
    return(1);
    }

