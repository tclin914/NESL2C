%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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
%token <node> IF THEN ELSE LET IN OR NOR XOR AND NAND RARROW LARROW NE EQ LE GE
%token <node> intconst floatconst ID boolconst stringconst TIME 
%token <node> '{' '}' '(' ')' ';' '=' ',' '[' ']' ':' '|' '$'


%type <node> goal TopLevel FunId EndMark FunTypeDef TypeExp PairTypes TypeList
%type <node> Exp IfOrLetExp ExpBinds ExpBind TupleExp TupleRest OrExp OrOp 
%type <node> AndExp AndOp RelExp RelOp AddExp AddOp MulExp MulOp ExpExp UnExp 
%type <node> UnOp SubscriptExp AtomicExp SpecialId ApplyBody RBinds RBind
%type <node> SequenceTail Const 

%type <node> '<' '>' '+' '-' PP 

%type <node> '*' '/' '^' '#' '@' 


%left ','
%left OR NOR XOR
%left AND NAND
%left EQ NE '<' '>' LE GE
%left '+' '-' PP LARROW
%left '*' '/' RARROW
%left '^' 
%left '#' '@' 
%nonassoc UMINUS

%start goal

%%

goal: TopLevel {ASTRoot=newNode(NODE_NESL);addChild(ASTRoot,$1);$$=ASTRoot; }
    | goal TopLevel {$$=$1; addChild($$,$2);}
    ;

TopLevel 
        : FUNCTION FunId Exp ':' FunTypeDef '=' Exp EndMark
        {
            $$=$1;
            $$->nodeType = NODE_FUNC;
            $1->string =  (char*)malloc(sizeof(strlen($2->string)));
            strcpy($$->string, $2->string);
            $6->nodeType = NODE_OP;
            $6->op = OP_BIND;
            
            //struct nodetype *pattern = newNode(NODE_PATTERN);
            //addChild(pattern,$3);
            //$3->nodeType = NODE_PATTERN;
            //addChild($$,pattern);
            addChild($$,$3);
            
            struct nodeType *types = newNode(NODE_FUNC_TYPE);
            //addChild(types, $5);
            //addChild($$,types);
            
            addChild($$,$5);
            //addChild($6,$7);
            //addChild($$,$6);
            addChild($$,$7);
            deleteNode($8);
        }
        | FUNCTION FunId Exp '=' Exp EndMark{
            $$ = $1;
            $$->nodeType = NODE_FUNC;
            $1->string =  (char*)malloc(sizeof(strlen($2->string)));
            strcpy($$->string, $2->string);
            $4->nodeType = NODE_OP;
            $4->op = OP_BIND;
            
            //struct nodetype *input = newNode(NODE_FUNC_INPUT);
            //addChild($$,input);
            addChild($$, $3);
            
            //struct nodetype *pattern = newNode(NODE_PATTERN);
            //addChild(pattern,$3);
            //$3->nodeType = NODE_PATTERN;
            //addChild($$,$3);
            //addChild($$,pattern);
            addChild($$,$5);
            deleteNode($6);
        }
        | DATATYPE ID '(' TypeList ')' EndMark{
            //FIXME float, int ... is also token ID
            // but different tokenType
            $$ = $1;
            $$->nodeType = NODE_DATATYPE;
            addChild($$,$2);
            addChild($$,$4);
            deleteNode($6);
        }
        | Exp '=' Exp EndMark {
            $$ = newNode(NODE_OP);
            $$->op = OP_BIND;
            addChild($$,$1);
            addChild($$,$3);
            deleteNode($4);
        }
        | Exp EndMark {
            $$ = $1;
            deleteNode($2);
        }
        ;

FunId   : ID{
            //FIXME float, int ... is also token ID
            // but different tokenType
            $$=$1;
        }
        | SpecialId{
            $$=$1;
        }
        ;

EndMark : ';' 
        | '$'
        ;

FunTypeDef : TypeExp RARROW TypeExp{   
            $$ = $2; 
            $$->nodeType = NODE_OP;
            $$->op = OP_RARROW;
            addChild($$,$1); 
            addChild($$,$3);
        }
        ;

TypeExp : ID {  
            //FIXME float, int ... is also token ID
            // but different tokenType
            switch($1->tokenType){
                case TOKE_INT:
                    $1->valueType = TypeInt;
                    break;
                case TOKE_FLOAT:
                    $1->valueType = TypeFloat;
                    break;
                case TOKE_BOOL:
                    $1->valueType = TypeBool;
                    break;
                case TOKE_CHAR:
                    $1->valueType = TypeChar;
                    break;
                default:
                    break;
            } 
            //if(strcmp($$->string,"float")==0){
            //    $1->valueType = TypeFloat;
            //}
            //else if(strcmp($$->string, "int")==0){
            //    $1->valueType = TypeInt;
            //}
            $$ = $1;  
        }
        | ID '(' TypeList ')' {
            //FIXME float, int ... is also token ID
            // but different tokenType
            $$=$1;
            addChild($$,$3);
        }
        | '[' TypeExp ']' {
            $$ = newNode(NODE_TYPE_SEQ);
            addChild($$,$2);
        }
        | '('PairTypes ')' {
            $$ = newNode(NODE_TYPE_PAIR);
            addChild($$,$2);
            //$$ = $2;
        }
        ;

PairTypes : TypeExp ',' PairTypes{
            //$$ = $1;
            //$2->nodeType = NODE_TOKEN;
            //$2->string = ",";
            ////addChild($$,$2);
            //addChild($$,$3);

            $$=newNode(NODE_TUPLE);
            addChild($$,$1);
            addChild($$,$3);
        }
        | TypeExp {
            //$$ = newNode(NODE_TYPE_PAIR);
            //addChild($$,$1);

            $$=$1;   
        }
        ;

TypeList : TypeList ',' TypeExp {
            $$ = newNode(NODE_LIST);
            addChild($$,$1);
            addChild($$,$3);
            deleteNode($2);
        }
        | TypeExp{
            $$ = $1;
            
        }
        ;

Exp : IfOrLetExp {
        $$ = $1;
    }
    | TupleExp {
       /* 
        if($1->child->child == NULL){
            $$->nodeType = NODE_TUPLE_HEAD;
            $$ = $1->child;
        }
        */
        /*
        if($1->nodeType!=NODE_OP){   
        $$ = newNode(NODE_TUPLE_HEAD);
        addChild($$,$1);
        }
        else $$=$1;
        */
        $$ = $1; 
     // $$=newNode(NODE_TUPLE);
     // addChild($$,$1);
    }
    ;

IfOrLetExp
    : IF Exp THEN Exp ELSE Exp {
        $$ = newNode(NODE_IFELSE);
        addChild($$,$1);
        addChild($$,$3);
        addChild($$,$5);
        $1->nodeType = NODE_IFSTMT;
        $3->nodeType = NODE_THENSTMT;
        $5->nodeType = NODE_ELSESTMT;
        addChild($1,$2);
        addChild($3,$4);
        addChild($5,$6);
    }
    | LET ExpBinds ';' IN Exp {
        $$= newNode(NODE_LET);
        addChild($$,$2);
        struct nodeType* LET_REST = newNode(NODE_EXP);
        addChild($$, LET_REST);
        addChild(LET_REST,$5);
    }
    | LET ExpBinds IN Exp{
        $$ = newNode(NODE_LET);
        addChild($$,$2);
        struct nodeType* LET_REST = newNode(NODE_EXP);
        addChild($$, LET_REST);
        addChild(LET_REST,$4);
    }
    ;

ExpBinds
    : ExpBind {
        $$ = newNode(NODE_BIND);
        addChild($$,$1);
    }
    | ExpBinds ';' ExpBind { 
        $$ = $1; 
        addChild($$,$3); 
        deleteNode($2); 
    }
    ;

ExpBind
    : Exp '=' Exp{
        $$ = $2;
        $$->nodeType = NODE_OP;
        $$->op = OP_BIND;
        struct nodeType *pattern = newNode(NODE_PATTERN);
        addChild($$,pattern);
        addChild(pattern, $1);
        addChild($$,$3);
    }
    ;

TupleExp
    : OrExp {
        //$$ = newNode(NODE_TUPLE);
        //addChild($$, $1);
        $$ = $1;
    }
    | OrExp ',' TupleRest {
        $$ = newNode(NODE_TUPLE);
        addChild($$,$1);
        addChild($$,$3);
        
        //$$ =$3;
        //addChild($$,$1);
        //$$ = $1;
        //addChild($$,$3);
        // FIXME
        // TODO
        // Reverse the tuple list.
    }
    ;


TupleRest
    : TupleExp{
        $$ = $1;
    }
    | IfOrLetExp{
        $$ = $1;
    }
    ;

OrExp
    : OrExp OrOp AndExp {
        $$ = $2;
        addChild($$,$1);
        addChild($$,$3);
    }
    | AndExp {
        $$ = $1;
    }
    ;

OrOp: OR  { $$=$1; $$->nodeType = NODE_OP; $$->op=OP_OR;  }
    | NOR { $$=$1; $$->nodeType = NODE_OP; $$->op=OP_NOR;  }
    | XOR { $$=$1; $$->nodeType = NODE_OP; $$->op=OP_XOR;  } ;

AndExp
    : RelExp {$$=$1;}
    | AndExp AndOp RelExp {
        $$=$2;
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

AndOp
    : AND  {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_AND;}
    | NAND {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_NAND;}
    ;

RelExp
    : AddExp {$$=$1;}
    | RelExp RelOp AddExp{
        $$ = $2;
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

RelOp
    :  EQ {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_EQ;}
    | NE  {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_NE;}
    | '<' {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_LT;}
    | '>' {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_GT;}
    | LE  {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_LE;}
    | GE  {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_GE;}
    ;

AddExp
    : MulExp {$$=$1;}
    | AddExp AddOp MulExp {
        $$ = $2; 
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

AddOp
    : '+'     {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_ADD;}
    | '-'     {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_SUB ;}
    | PP      {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_PP ;}
    | LARROW  {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_LARROW ;}
    ;

MulExp
    : ExpExp {$$=$1;}
    | MulExp MulOp ExpExp {
        $$ = $2; 
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

MulOp 
    : '*'    {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_MUL; } 
    | '/'    {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_DIV; } 
    | RARROW {$$=$1; $$->nodeType = NODE_OP; $$->op = OP_RARROW; }  
    ;

ExpExp 
    : UnExp { $$=$1;}
    | ExpExp '^' UnExp {
        $$ = $2; 
        $$->nodeType = NODE_OP; $$->op = OP_UPT;
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

UnExp 
    : SubscriptExp {$$=$1;}
    | UnOp UnExp   {$$=$1; addChild($$,$2);}
    ;

UnOp
    : '#'{$$=$1; $$->nodeType = NODE_OP; $$->op = OP_SHARP;}
    | '@'{$$=$1; $$->nodeType = NODE_OP; $$->op = OP_AT;}
    | '-'{$$=$1; $$->nodeType = NODE_OP; $$->op = OP_UMINUS;} 
    ;

SubscriptExp
    : AtomicExp {$$ = $1;}
    | AtomicExp '[' Exp ']'{
        $$=newNode(NODE_SEQ_REF); 
        //$$->string = (char*)malloc(strlen($1->string));
        //strcpy($$->string,$1->string);
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

AtomicExp
    : Const {
        $$=$1;
    }
    | SpecialId '(' Exp ')' {
       $$ = newNode(NOTE_IMPLEMENT); 
    }
/*  TODO extract TIME(EXP) FLOAT(EXP) */
    | '{' ApplyBody '}' {
        $$ = $2;
    }
    | '{' ApplyBody '|' Exp '}' {
        if($2->nodeType == NODE_APPLYBODY1){
            $2->nodeType =NODE_APPLYBODY3;
            //addChild($2,$4);
        }
        else {
            $2->nodeType =NODE_APPLYBODY4;
           //` addChild($2,$4);
        }
        $$ = $2;
        struct nodeType* filter = newNode(NODE_FILTER);
        addChild($$,filter);
        addChild(filter, $4);
    }
    | '[' ']' TypeExp {
        //Empty Sequence
        $$ = newNode(NODE_EMPSEQ);
        addChild($$,$1);
    }
    | '[' Exp SequenceTail ']'{
        if($2->nodeType!=NODE_TUPLE){
        $$ = newNode(NODE_SEQ);
        addChild($$,$2);
        //addChild($$,$3);   
        }
        else{ 
            $$ =$2;
            $$->nodeType=NODE_NEW_SEQ;
        }
        if($3->nodeType!=NODE_EMPTY)
          addChild($$,$3);   
    }
    | '(' Exp ')' {
        $$ = newNode(NODE_PAIR);
        addChild($$,$2);
        //$$ = $2;

    }
    | ID {
            //FIXME float, int ... is also token ID
            // but different tokenType
        $$ = $1;
    }
    | ID '(' Exp ')'{
            //FIXME float, int ... is also token ID
            // but different tokenType
        $$ = newNode(NODE_FUNC_CALL);
        addChild($$,$1);
        struct nodeType *pair = newNode(NODE_PAIR);
        addChild(pair, $3);
        addChild($$,pair);
    }
    ;

SpecialId
    : ANY {
        $$ = $1; 
        $$->nodeType = NOTE_IMPLEMENT;
    }
    ;

ApplyBody
    : Exp ':' RBinds{
        
        $$ = newNode(NODE_APPLYBODY2);           
        addChild($$,$1);
        addChild($$,$3);
    }
    | RBinds {
        $$ = newNode(NODE_APPLYBODY1);
        addChild($$,$1);
    }
    ;

RBinds
    : RBinds ';' RBind{
        $$ = $1; 
        //addChild($$,$3);
        //addChild($$,$1);
        addChild($$,$3);
        deleteNode($2);
    
    }
    | RBind {
        $$ = newNode(NODE_RBINDS);
        //$$ =$1;
        addChild($$,$1);
    }
    ;
    
RBind
    : ID {
        //FIXME float, int ... is also token ID
        // but different tokenType
        $$ = $1;
    }
    | Exp IN Exp{
        $$ = $2;
        $$->nodeType = NODE_IN;
        //$1->nodeType = NODE_TOKEN;
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

SequenceTail
    : ':' Exp {
        $$ = $2;
        $$->nodeType = NODE_SEQTAIL;
    }
    | ':' Exp ':' Exp {
        $$ = $2; 
        addChild($$,$4);

    }
    | {
        $$ = newNode(NODE_EMPTY);
    } 
    ;

Const
    : intconst{
        $$ = $1;
        $$->nodeType = NODE_INT;
    }
    | floatconst{
        $$ = $1;
        $$->nodeType = NODE_FLOAT;
    }
    | boolconst{
        $$ = $1;
        $$->nodeType = NODE_BOOL;
    }
    | stringconst{
        //FIXME change into SEQ of CHAR.
        $$ = $1;
        $$->nodeType = NODE_STRING;
    }
    ;

%%

extern void removePair(struct nodeType *node);
extern void printNESL(struct nodeType *node, FILE* yyout);
extern void semanticPass( struct nodeType *node);

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
    printf("%d: %s\n",argc,argv[1]);
    yyin = fopen(argv[1], "r");
    
    /**
    * Extract filename from argument.
    */
    char *classname;
    classname = (char*)malloc(sizeof(char)*100);
    classname = strtok(argv[1],"/.");
    classname = strtok(NULL,"/.");
    
    /**
    * Parse
    */
    yyparse();
    fclose(yyin);
    printf("************************\n");
    printf("*** NO PARSING ERROR ***\n");
    printf("************************\n");

    /**
    * PrintTree
    */
    printTree(ASTRoot, 0);
    removePair(ASTRoot);
    printTree(ASTRoot,0);
    
    /**
    * Generate NESL to compare difference.
    */
    char *reveseNESL ;
    reveseNESL = (char*)malloc(sizeof(char)*100);
    strcpy(reveseNESL,"reverseoutput/");
    strcat(reveseNESL, classname);
    strcat(reveseNESL, ".nesl");
    printf("%s\n",reveseNESL);
    yyout = fopen(reveseNESL,"w+");
    printNESL(ASTRoot, yyout); 
    fclose(yyout);
    printf("************************\n");
    printf("** REVERSE NESL DONE ***\n");
    printf("************************\n");
   


    /**
    * Semantic Check: type
    */
    // TODO 
    semanticPass(ASTRoot);
    
    //semanticCheck(ASTRoot);
    //printf("************************\n");
    //printf("** NO SEMANTIC ERROR ***\n");
    //printf("************************\n");

    /**
    * Generate C file.
    */
    char *translatedC = (char*)malloc(sizeof(char)*100);
    strcpy(translatedC, "output/");
    strcat(translatedC, classname);
    strcat(translatedC,".c");
    
    yyout = fopen(translatedC,"w+");
    codegen(yyout, ASTRoot);
    fclose(yyout);
    
    
    //FILE* fptr;
    //fptr = fopen("output/NESL2C_test.c","w");
    //if(!fptr){
    //    printf("failed create output file! exit\n");
    //    exit(1);
    //}
    // fprintf(fptr, "This is a test\n");
    //fprintf(fptr, "#include<stdio.h>\n#include<stdlib.h>\n\nint main(){\n\t");

    //codegen(fptr, ASTRoot);

    //fprintf(fptr, "\n}");
    //printf("************************\n");
    //printf("***** CODEGEN DONE *****\n");
    //printf("************************\n");
    //if(fptr==NULL)
    //    fclose(fptr);
    
    printf("************************\n");
    printf("***  END OF NESL2C  ****\n");
    printf("************************\n");

    return 0;
}
