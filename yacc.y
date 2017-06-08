%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "lex.yy.c"
#include "node.h"
#include "symtab.h"
#include "assert.h"
#include "codegen.h"
#include "pfcodegen.h"
#include "sqcodegen.h"
#include "codegencheck.h"

int yydebug =1;
int yyerror(const char *s);
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
            $6->nodeType = NODE_ASSIGN;
            
            //struct nodeType *pattern = newNode(NODE_PATTERN);
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
            $4->nodeType = NODE_ASSIGN;
            
            addChild($$, $3);
            
            addChild($$,$5);
            deleteNode($6);
        }
        | DATATYPE ID '(' TypeList ')' EndMark{
            $$ = $1;
            $$->nodeType = NODE_DATATYPE;
            addChild($$,$2);
            addChild($$,$4);
            deleteNode($6);
        }
        | Exp '=' Exp EndMark {
            $$ = newNode(NODE_ASSIGN);
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
            /* if(tokenType!=TOKE_STRING) yyerror();*/
            switch($1->tokenType){
            case TOKE_INT:
            case TOKE_FLOAT:
            case TOKE_BOOL:
            case TOKE_CHAR: 
              yyerror($1->string); 
            break;
            }
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
            $$ = $1;  
        }
        | ID '(' TypeList ')' {
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
        $$->nodeType = NODE_ASSIGN;
        struct nodeType *pattern = newNode(NODE_PATTERN);
        addChild($$,pattern);
        addChild(pattern, $1);
        addChild($$,$3);
    }
    ;

TupleExp
    : OrExp {
        $$ = $1;
    }
    | OrExp ',' TupleRest {
        $$ = newNode(NODE_TUPLE);
        addChild($$,$1);
        addChild($$,$3);
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
       switch($1->tokenType){
       case TOKE_INT:
       case TOKE_FLOAT:
       case TOKE_BOOL:
       case TOKE_CHAR: 
         yyerror($1->string); 
       break;
       }
       
       $$ = $1;
    }
    | ID '(' Exp ')'{
            //FIXME float, int ... is also token ID
            // but different tokenType
        switch($1->tokenType){
         case TOKE_INT:
         case TOKE_BOOL:
         case TOKE_CHAR: 
           yyerror($1->string); 
         break;
         }
        
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
        yyerror("not implemented");
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
        addChild($$,$3);
        deleteNode($2);
    
    }
    | RBind {
        $$ = newNode(NODE_RBINDS);
        addChild($$,$1);
    }
    ;
    
RBind
    : ID {
        switch($1->tokenType){
         case TOKE_INT:
         case TOKE_FLOAT:
         case TOKE_BOOL:
         case TOKE_CHAR: 
           yyerror($1->string); 
         break;
         }
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
        $$ = $1;
        $$->nodeType = NODE_STRING;
        $$->valueType = TypeSEQ_C;
    }
    ;

%%

extern void removePair(struct nodeType *node);
extern void printNESL(struct nodeType *node, FILE* yyout);
extern void semanticPass( struct nodeType *node);

int ispfc=0;
int issqc=0;
int isomp=0;
int isrev=0;

struct nodeType *ASTRoot;
struct nodeType * newOpNode(int op) {
    struct nodeType *node = newNode(NODE_OP);
    node -> op = op;
    return node;
}

int yyerror(const char *s) {
    printf("Syntax error: %s \n",s);
    //printf("errror: %s",s);

    exit(0);
}

int main(int argc, char** argv){
    
    char *classname;
    classname = (char*)malloc(sizeof(char)*100);
    int usename=0; 
    // extract the filename option from arguments.
    // file.nesl -PF -
    // 0th argument is ./NESL2C itself
    assert(argc);
    for(int i =0;i<argc;i++){ 
        printf("%d: %s\n",i,argv[i]);
        char c = argv[i][0];
        printf("%c\n", c);
        if(i >0){
            //printf("%d:%c\n",i,argv[i][0]);
            if(c== '-'){
                // option flags.
                if(!strcmp(argv[i], "-rev")){
                    isrev = 1;
                    printf("%d set rev\n",i);
                }
                if(!strcmp(argv[i], "-pfc")){
                    ispfc = 1;
                    printf("%d set pfc\n",i);
                }
                if(!strcmp(argv[i], "-sqc")){
                    issqc = 1;
                    printf("%d set sqc\n",i);
                }
                if(!strcmp(argv[i], "-omp")){
                    isomp = 1;
                    printf("%d set omp\n",i);
                }
                if(!strcmp(argv[i], "-o")){
                    usename = 1;
                    //printf("setfilename :%s\n", argv[i+1]);
                    strcpy(classname,argv[i+1]);
                    printf("setfilename :%s\n", classname);
                    i++;
                }
            }
            else{
                printf("%d: %s\n",argc,argv[1]);
                yyin = fopen(argv[i], "r");
                /* Extract filename from argument.*/
                classname = strtok(argv[1],"/.");
                classname = strtok(NULL,"/.");
            }
        }
    }

    /**
    * Parse
    */
    //yyin = fopen(argv[1], "r");
    assert(yyin);
    yyparse();
    fclose(yyin);
    printf("************************\n");
    printf("*** NO PARSING ERROR ***\n");
    printf("************************\n");

    /**
    * PrintTree
    */
    printTree(ASTRoot,0);
    printf("************************\n");
    
    /**
    * Generate NESL to compare difference.
    */
    char *reveseNESL ;
    reveseNESL = (char*)malloc(sizeof(char)*100);
    if(isrev){
        if(!usename){
        strcpy(reveseNESL,"reverseoutput/");
        strcat(reveseNESL, classname);
        strcat(reveseNESL, ".nesl");
        printf("%s\n",reveseNESL);
        }
        else{
        sprintf(reveseNESL,"%s.nesl",classname);
        }
        yyout = fopen(reveseNESL,"w+");
        printNESL(ASTRoot, yyout); 
        fclose(yyout);
        printf("************************\n");
        printf("** REVERSE NESL DONE ***\n");
        printf("************************\n");
    }
    free(reveseNESL);
    
    /**
    * Semantic Check: type
    */
    // TODO 
    semanticPass(ASTRoot);
     
    printf("************************\n");
    printf("** NO SEMANTIC ERROR ***\n");
    printf("************************\n");

    /**
    * Generate C file.
    */
    char *translatedC = (char*)malloc(sizeof(char)*100);
    if(ispfc||issqc||isomp){
      if(issqc){
        if(!usename){
        sprintf(translatedC, "output/%s_sqc.c",classname);
        }else{
        strcpy(translatedC, classname);
        }
        yyout = fopen(translatedC,"w+");
        
        // print Time information.
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(yyout, "/** \n* genereated by NESL2C from %s.nesl:\n* GMT+8: %d-%d-%d %d:%d:%d\n*/\n\n",classname, 
                            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
                            tm.tm_hour, tm.tm_min, tm.tm_sec);
        fprintf(yyout, "#include <stdio.h>\n#include <stdlib.h>\n#include \"sqmacro.h\"\n");
        //sqcheck(ASTRoot);
        pfcheck(ASTRoot);
        
        for(int i=0;i<100;i++){
          strcpy(refTable.entries[i].name, "");
        }
        sqcodegen(yyout, ASTRoot);
        //pfcodegen(yyout, ASTRoot);
        //codegen(yyout, ASTRoot);
        fclose(yyout);    
      }
      if(ispfc){
        if(!usename){
        sprintf(translatedC, "output/%s_pfc.c",classname);
        }else{
        strcpy(translatedC, classname);
        }
        yyout = fopen(translatedC,"w+");
        
        // print Time information.
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(yyout, "/** \n* genereated by NESL2C from %s.nesl:\n* GMT+8: %d-%d-%d %d:%d:%d\n*/\n\n",classname, 
                            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
                            tm.tm_hour, tm.tm_min, tm.tm_sec);
        fprintf(yyout, "#include <stdio.h>\n#include <stdlib.h>\n#include \"pf/pf.h\"\n#include \"pfmacro.h\"\n");
        //fprintf(yyout, "struct Pair_F {\n\tfloat a;\n\tfloat b;\n};\n\n");
        //fprintf(yyout, "struct Sequence {\n\tint len;\n\tint cap;\n\tvoid *ptr;\n};\n\n");
        pfcheck(ASTRoot);
        //refTable.size = 100;
        for(int i=0;i<100;i++){
          strcpy(refTable.entries[i].name, "");
        }

        //pfcodegen(yyout, ASTRoot); 
        sqcodegen(yyout, ASTRoot); 
        fclose(yyout);
      }
      if(isomp){
        strcpy(translatedC, "output/");
        strcat(translatedC, classname);
        strcat(translatedC, "_omp");
        strcat(translatedC,".c");
        yyout = fopen(translatedC,"w+");
        
        // print Time information.
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        fprintf(yyout, "/** \n* genereated by NESL2C from %s.nesl:\n* GMT+8: %d-%d-%d %d:%d:%d\n*/\n\n",classname, 
                            tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
                            tm.tm_hour, tm.tm_min, tm.tm_sec);
        fclose(yyout);
      }
    }
    free(translatedC);
    printf("************************\n");
    printf("***  END OF NESL2C  ****\n");
    printf("************************\n");

    return 0;
}
