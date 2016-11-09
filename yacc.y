%{
#include <stdlib.h>
#include <stdio.h>
#include "lex.yy.c"
#include "node.h"
#include "symtab.h"

int yydebug =1;

struct nodeType * newOpNode(int op); 
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
%token <node> '{' '}' '(' ')' ';' '=' ',' '[' ']' ':' '|'

%type <node> goal nesl toplevel funcopt typedef typebinds typebind
%type <node> typeexp typelist typeclass basetype exp expoption1
%type <node> expoption2 expbinds expbindsoption pattern rbinds
%type <node> rbind sequence seqoption explist const binop unaryop


%left ','
%left OR NOR XOR
%left AND NAND
%left EQQ NEQQ '<' '>' LE GE
%left '+' '-' PP LARROW
%left '*' '/' RARROW
%left '^' 
%left '#' '@' 
%nonassoc UMINUS

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

toplevel : FUNCTION nametoken pattern ':' typedef '=' exp ';' {
                $$=newNode(NODE_EXP);
                addChild($$, $2);
                addChild($$, $3);
                addChild($$, $4);
            }
    |   DATATYPE nametoken typedef ';' {
            $$=newNode(NODE_DATATYPE);
            addChild($$,$2);
            addChild($2,$3);
        }
    |   pattern '=' exp ';' {
                $$ = newNode(NODE_ASSIGN_STMT);
                addChild($$, $1);
                addChild($$, $3);
        }
    |   exp ';' {
            $$=newNode(NODE_EXP);
    


// funcopt : ':' typedef '=' exp ';' {
//     $$=newNode(NODE_EXP);
//     addChild($$, $2);
//     addChild($$, $4);
// }
// ;
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

typebinds : typebind {
            $$=$1;
        }
        | typebind ';' typebinds{
            $$=$3;
            addChild($$,$1);
        }
        ;

typebind : nametoken IN typeclass {
            $$=$3;
            addChild($$,$1);
        }
        ;

typeexp : basetype {
                $$=$1;
        }
        | nametoken {
                $$=$1;
        }
        | typeexp RARROW typeexp {
                //TODO weird.
                $$=$3;
                addChild($$,$1);
        }
        | typeexp ',' typeexp {
                //TODO weird.
                $$=newNode(NODE_LIST);
                addChild($$,$1);
                addChild($$,$3);
        }
        | nametoken '(' typelist ')' {
                $$=newNode(NODE_FUNC); 
                addChild($$,$1); 
                addChild($$,$3);
        }
        | '[' typeexp ']' {$$=$2;}
        | '(' typeexp ')'{$$=$2;}
        ;

typelist : typeexp {
            $$ = newNode(NODE_LIST);
            addChild($$,$1);
            } 
        | typeexp ',' typelist {
                $$=$3;
                addChild($$,$1);
        }
         ;

typeclass : NUMBER {$$=$1;}
        | ORDINAL  {$$=$1;}
        | LOGICAL  {$$=$1;}
        | ANY      {$$=$1;}
        ;

basetype : INT {$$=$1;}
        | BOOL {$$=$1;}
        | FLOAT {$$=$1;}
        | CHAR {$$=$1;}
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
    | '{'{printf("\t\t\t\tapplytoeach\n");} applytoeach '}' {
            printf("\texp rule3\n");
           /* $$=newNode(NODE_EXP);
            if($1->nodeType != NODE_EMPTY)
                addChild($$,$1);
            if($3->nodeType != NODE_EMPTY)
                addChild($$,$3);
        */
    }
    | '(' explist ')' {
            printf("\texp exp : function application\n");
            $$=newNode(NODE_EXP);
            //addChild($$,$1);
            addChild($$,$2);
    }
    | nametoken '(' explist ')' {
            printf("\texp exp : function application\n");
            $$=newNode(NODE_EXP);
            //addChild($$,$1);
            addChild($$,$2);
    }
    | sequence{ 
        printf("\tSEQUENCE\n");
        $$=$1;
        $$ -> nodeType=NODE_SEQ;
    }
    | exp binop exp {
            printf("\texp_binary_Operation_reduce\n"); 
            $$=$2;
            addChild($$,$1);
            addChild($$,$3);
    }
    | unaryop exp{
            printf("\texp_unaryop\n");
            $$=$1;
            addChild($$,$2);
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

applytoeach : expoption1  rbinds expoption2
    |   rbinds 
    |   rbinds  expoption2
    |   expoption1 rbinds
    ;
expoption1 : nametoken '(' pattern  ')' {printf("opt1`111\n");}  ':' {
        printf("\t\t\t\texpoptinal1\n");
        $$ =$1;
    }
    | {
        $$=newNode(NODE_EMPTY);
    }
    ;

expoption2 : '|' exp{
            $$=newNode(NODE_EXP);
            printf("\t\t\t\texpoption2\n");
            }
    | {$$=newNode(NODE_EXP);}
    ;

expbinds :
    pattern '=' exp ';' expbindsoption  {$$=$5; addChild($$,$1); addChild($1,$3);}
    ;

expbindsoption :  expbinds {
            printf("\t\t\t\tbindbind\n");
            $$=$1;
            }
    | {$$=newNode(NODE_LIST);}
    ;

rbinds : rbind {printf("\t\t\t\t rbind rbinds\n");}  rbinds{$$=newNode(NODE_EXP);}
    | ';' rbind{$$=newNode(NODE_EXP);}
    | {$$=newNode(NODE_EXP); printf("\t\t\t\tempty rbinds\n");}
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

explist : explist ',' exp { 
            addChild($1, $3);
            $$=$1;
        }
    | exp { 
            $$ = newNode(NODE_LIST);
            addChild($$,$1);
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

binop : 
    {
    /*
        ','{
            $$=newOpNode(OP_COMMA);
        }
    
    |*/ 
    }
    OR {$$=newOpNode(OP_OR);}| NOR {$$=newOpNode(OP_NOR);}| XOR{$$=newOpNode(OP_XOR);}
    | AND {$$=newOpNode(OP_AND);}| NAND {$$=newNode(OP_NAND);}
    | EQQ {$$=newOpNode(OP_EQQ);}| NEQQ {$$=newOpNode(OP_DIVEQ);}| '<' {$$=newOpNode(OP_LT);}
        | '>' {$$=newOpNode(OP_GT);}| LE {$$=newOpNode(OP_LE);}| GE {$$=newOpNode(OP_GE);}
    
    | '+' {$$=newNode(NODE_EXP);} | '-' {$$=newNode(NODE_EXP);}| PP {$$=newNode(NODE_EXP);}
        | LARROW {$$=newOpNode(OP_LARROW);}
    
    | '*' {$$=newOpNode(OP_MUL);} | '/' {$$=newOpNode(OP_DIV);}| RARROW {$$=newOpNode(OP_RARROW);}
    | '^' {$$=newOpNode(OP_UPT);}
    ;

unaryop :   '#' {$$=newOpNode(OP_SHARP);}
    |   '@'  {$$=newOpNode(OP_AT);}
    |   '-' %prec UMINUS {$$=newOpNode(OP_UMINUS);}
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
