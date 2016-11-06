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
%token <node> '{' '}' '(' ')' ';' '=' ',' '[' ']' ':' '+' '-' '*' '/' '>'

%type <node> goal nesl toplevel funcopt typedef typebinds typebind
%type <node> typeexp typelist typeclass basetype exp expoption1
%type <node> expoption2 expbinds expbindsoption pattern rbinds
%type <node> rbind sequence seqoption explist const binop unaryop

%start goal

%%

goal :  nesl {ASTRoot = $1;}
    ;

nesl : nesl toplevel { 
            printf("toplevel\n"); 
            $$=$1;
            addChild($1,$2);
        }
        | {
            $$=newNode(NODE_NESL);
        }
        ;

toplevel : FUNCTION nametoken pattern funcopt {
                $$=newNode(NODE_EXP);
                addChild($$, $2);
                addChild($$, $3);
                addChild($$, $4);
            }
    |   DATATYPE nametoken typedef ';' {
            $$=newNode(NODE_EXP);
        }
    |   pattern '=' exp {
                $$ = newNode(NODE_ASSIGN_STMT);
                addChild($$, $1);
                addChild($$, $3);
        }
    |   exp {
            $$=newNode(NODE_EXP);
        }
    ;

funcopt : ':' typedef '=' exp ';' {
            $$=newNode(NODE_EXP);
            addChild($$, $2);
            addChild($$, $4);
        }
        ;

pattern : nametoken {
            $$->nodeType = NODE_SYM_REF;
            $$=$1;
        }
    |   nametoken '(' pattern ')' {$$=$1; }
    |   pattern ',' pattern {$$=newNode(NODE_EXP);}
    |   '(' pattern ')' {$$=newNode(NODE_EXP);}
    ;

typedef : typeexp {$$=newNode(NODE_EXP);}|
        typeexp ':'':' '(' typebinds ')'{$$=newNode(NODE_EXP);}
        ;

typebinds : typebind {$$=newNode(NODE_EXP);}|
        typebind ';' typebinds{$$=newNode(NODE_EXP);}
        ;

typebind : nametoken IN typeclass {$$=newNode(NODE_EXP);}
        ;

typeexp : basetype {$$=newNode(NODE_EXP);}|
        nametoken {$$=newNode(NODE_EXP);}|
        typeexp '-''>' typeexp {$$=newNode(NODE_EXP);}|
        typeexp ',' typeexp {$$=newNode(NODE_EXP);}|
        nametoken '(' typelist ')' {$$=newNode(NODE_EXP);}|
        '[' typeexp ']' {$$=newNode(NODE_EXP);}|
        '(' typeexp ')'{$$=newNode(NODE_EXP);}
        ;

typelist : typeexp {$$=newNode(NODE_EXP);} |
           typeexp ',' typelist {$$=newNode(NODE_EXP);}
         ;

typeclass : NUMBER {$$=newNode(NODE_EXP);}
        | ORDINAL {$$=newNode(NODE_EXP);}
        | LOGICAL {$$=newNode(NODE_EXP);}
        | ANY {$$=newNode(NODE_EXP);}
        ;

basetype : INT {$$=newNode(NODE_EXP);}
        | BOOL {$$=newNode(NODE_EXP);}
        | FLOAT {$$=newNode(NODE_EXP);}
        | CHAR {$$=newNode(NODE_EXP);}
        ;

exp : const {$$ = $1;}
    | nametoken {
        printf("\texp nametoken\n");
        $$ = $1;
    }
    | IF exp THEN exp ELSE exp {
            printf("\texp IFELSE\n");
            $$=newNode(NODE_IFELSE);
            $2->nodeType=NODE_IFSTMT;
            addChild($$,$2);
            $4->nodeType = NODE_THENSTMT;
            addChild($$,$4);
            $6->nodeType = NODE_ELSESTMT;
            addChild($$,$6);
    }
    | LET expbinds IN exp {
            $$=newNode(NODE_EXP);
            printf("\t exp LET\n");
            addChild($$,$2);
            addChild($$,$4);
    }
    | '{' expoption1 rbinds expoption2 '}' {
            printf("\texp rule3\n");
            $$=newNode(NODE_EXP);
            if($1->nodeType != NODE_EMPTY)
                addChild($$,$1);
            if($3->nodeType != NODE_EMPTY)
                addChild($$,$3);
    }
    | exp exp {
            printf("\texp exp : function application\n");
            $$=newNode(NODE_EXP);
            addChild($$,$1);
            addChild($$,$2);
    }
    | exp binop exp {
            printf("\texp_binary_Operation_reduce\n"); 
            $$=newNode(NODE_EXP);    
    }
    | unaryop exp{
            printf("\texp_unaryop\n");
            $$=newNode(NODE_EXP);
            addChild($$,$1);
            addChild($$,$2);
    }
    | sequence{ 
        printf("\tSEQUENCE\n");
        $$=$1;
        $$ -> nodeType=NODE_SEQ;
    }
    | exp '[' exp ']'{
            printf("\texp of sequence indexing \n");
            $$=newNode(NODE_EXP);
            addChild($$,$1);
            addChild($$,$3);
    }
    | '(' exp ')'{
            printf("\texp with () \n");
            $$=newNode(NODE_EXP);
            addChild($$,$2);
    }
    ;

expoption1 : exp ':' {
        printf("\texpoptinal1\n");
        $$ =$1;
    }
    | {
        $$=newNode(NODE_EMPTY);
    }
    ;

expoption2 : '|' exp{$$=newNode(NODE_EXP);}
    | {$$=newNode(NODE_EXP);}
    ;

expbinds :
    pattern {printf("LET exb\n");} '=' exp ';' expbindsoption  {$$=newNode(NODE_EXP);}
    ;

expbindsoption :  {printf("LET epp\n");} expbinds {$$=newNode(NODE_EXP);}
    | {$$=newNode(NODE_EXP);}
    ;

rbinds : rbind rbinds{$$=newNode(NODE_EXP);}
    | ';' rbind{$$=newNode(NODE_EXP);}
    |{$$=newNode(NODE_EXP);}
    ;

rbind : pattern IN exp {$$=newNode(NODE_EXP);}
    | nametoken {$$=newNode(NODE_EXP);}
    ;

sequence : '[' explist ']'{
            $$=$2; printf("S way\n");
        }
    | '[' ']' typeexp{$$=newNode(NODE_EXP); printf("B way\n");}
    | '[' exp ':' exp seqoption ']'{$$=newNode(NODE_EXP); printf("B way\n");}
    ;
seqoption : ':' exp{$$=newNode(NODE_EXP);}
    ;

explist : explist ',' exp { printf(", way\n");
            printf("exp->nodeType: %d\n",$3->nodeType);
            addChild($1, $3);
            $$=$1;
        }
    | exp { printf("E way\n");
            $$ = newNode(NODE_LIST);
            addChild($$,$1);
            printf("exp->nodeType: %d\n",$1->nodeType);
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

binop : ','{$$=newNode(NODE_OP);}
    | OR {$$=newNode(NODE_EXP);}| NOR {$$=newNode(NODE_EXP);}| XOR{$$=newNode(NODE_EXP);}
    | AND {$$=newNode(NODE_EXP);}| NAND{$$=newNode(NODE_EXP);}
    | '=''=' {$$=newNode(NODE_EXP);}| '/''=' {$$=newNode(NODE_EXP);}| '<' {$$=newNode(NODE_EXP);}| '>' {$$=newNode(NODE_EXP);}| "<=" {$$=newNode(NODE_EXP);}| ">="{$$=newNode(NODE_EXP);}
    | '+' {$$=newNode(NODE_EXP);} | '-' {$$=newNode(NODE_EXP);}| '+' '+' {$$=newNode(NODE_EXP);}| "<-"{$$=newNode(NODE_EXP);}
    | '*' {$$=newNode(NODE_EXP);} | '/' {$$=newNode(NODE_EXP);}| '-' '>'{$$=newNode(NODE_EXP);}
    | '^' {$$=newNode(NODE_EXP);}
    ;

unaryop :   '#' {$$=newNode(NODE_EXP);}
    |   '@'  {$$=newNode(NODE_EXP);}
    |   '-' {$$=newNode(NODE_EXP);}
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
    
    semanticCheck(ASTRoot);
    printf("************************\n");
    printf("** NO SEMANTIC ERROR ***\n");
    printf("************************\n");

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
