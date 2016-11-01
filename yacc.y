%{
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "node.h"
#include "symtab.h"

int yydebug =1;

struct nodeType * nodeOpNode(int op);
extern struct nodeType* ASTRoot;
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

goal : toplevel  {ASTRoot = $1;}
    ;

nesl : toplevel { printf("toplevel\n"); $$=newNode(NODE_ERROR);}
        | {$$=newNode(NODE_ERROR);}
        ;

toplevel : FUNCTION nametoken pattern funcopt {$$=newNode(NODE_ERROR);}
    |   DATATYPE nametoken typedef ';' {$$=newNode(NODE_ERROR);}
    |   pattern '=' exp {
                printf("toplevel\n");
                $$ = newNode(NODE_ASSIGN_STMT);
                addChild($$, $1);
                addChild($$, $3);
        }
    |   exp {  $$=newNode(NODE_ERROR);}
    ;

funcopt : ':' typedef '=' exp ';' {$$=newNode(NODE_ERROR);}
        |{$$=newNode(NODE_ERROR);}
        ;

pattern : nametoken {
            $$->nodeType = NODE_SYM_REF;
            $$=$1;
        }
    |   nametoken '(' pattern ')' {$$=$1; }
    |   pattern ',' pattern {$$=newNode(NODE_ERROR);}
    |   '(' pattern ')' {$$=newNode(NODE_ERROR);}
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

exp : const {$$ = $1;}
    | nametoken {
        $$ = $1;
    }
    | IF exp THEN exp ELSE exp {$$=newNode(NODE_ERROR);}
    | LET expbinds IN exp {$$=newNode(NODE_ERROR);}
    | '{' expoption1 rbinds expoption2 '}' {$$=newNode(NODE_ERROR);}
    | exp exp{$$=newNode(NODE_ERROR);}
    | exp binop exp{$$=newNode(NODE_ERROR);}
    | unaryop exp{$$=newNode(NODE_ERROR);}
    | sequence{
        $$=$1;
        $$ -> nodeType=NODE_SEQ;
    }
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

sequence : '[' explist ']'{
            $$=$2;
        }
    | '[' ']' typeexp{$$=newNode(NODE_ERROR);}
    | '[' exp ':' exp seqoption ']'{$$=newNode(NODE_ERROR);}
    ;
seqoption : ':' exp{$$=newNode(NODE_ERROR);}
    ;

explist : exp explist {
            printf("exp->nodeType: %d\n",$1->nodeType);
            addChild($2, $1);
            $$=$2;
        }
    | ',' explist {
            $$=$2;
        }
    |   {
            $$ = newNode(NODE_LIST);
        }
    ;

const : intconst {
            $1->nodeType=NODE_INT;
            $$ = $1;
    }
    |   floatconst {$$=$1;}
    |   boolconst {$$=$1;}
    |   stringconst{$$=$1;}
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

int main(int argc, char** argv){
    printf("%d: %s\n",argc,argv[0]);
    
    yyparse();
    printf("************************\n");
    printf("*** NO PARSING ERROR ***\n");
    printf("************************\n");
    
    printTree(ASTRoot, 0);

    FILE* fptr;
    fptr = fopen("output/NESL2C_test.c","w");
    if(!fptr){
        printf("failed create output file! exit\n");
        exit(1);
    }
    // fprintf(fptr, "This is a test\n");
    fprintf(fptr, "#include<stdio.h>\n#include<stdlib.h>\n\nint main(){\n\t");

    codegen(fptr, ASTRoot);

    fprintf(fptr, "\n}");
    printf("************************\n");
    printf("***** CODEGEN DONE *****\n");
    printf("************************\n");
    if(fptr==NULL)
        fclose(fptr);
    
    return 0;
}
