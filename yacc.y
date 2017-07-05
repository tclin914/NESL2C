%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <libgen.h>

#include "lex.yy.c"
#include "node.h"
#include "symtab.h"
#include "assert.h"
#include "codegen.h"
#include "pfcodegen.h"
#include "sqcodegen.h"
#include "codegencheck.h"

#define DECHEAD "#include <stdio.h>\n#include <stdlib.h>\n#include \"sqmacro.h\"\n"
#define ENDOFMAIN "SET_HEAP_SIZE(MALLOC_HEAP_SIZE);\nmyFunc1();\ncheckglobal();\nCUDA_ERROR_CHECK();\nreturn 1;\n}\n"
int yydebug =1;
int yyerror(const char *s);
struct nodeType * newOpNode(enum OpType op); 
extern struct nodeType* ASTRoot;
%}

%union {
    struct nodeType *node;
    int number;
    int tokenval;
    int ivalue;
    float rvalue;
    char *string;

}

%token <tokenval> FUNCTION
%token <tokenval> T_INT T_BOOL T_FLOAT T_CHAR 
%token <ivalue> intconst
%token <rvalue> floatconst
%token <ivalue> boolconst
%token <string> stringconst
%token <string> ID

%token <tokenval> DATATYPE NUMBER ORDINAL LOGICAL ANY
%token <tokenval> IF THEN ELSE LET IN OR NOR XOR AND NAND RARROW LARROW NE EQ LE GE


%type <node> id 
%type <node> goal TopLevel FunId FunTypeDef TypeExp PairTypes TypeList
%type <node> Exp IfOrLetExp ExpBinds ExpBind TupleExp TupleRest OrExp OrOp 
%type <node> AndExp AndOp RelExp RelOp AddExp AddOp MulExp MulOp ExpExp UnExp 
%type <node> UnOp SubscriptExp AtomicExp SpecialId ApplyBody RBinds RBind
%type <node> SequenceTail Const 



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
            $$ = newNode(NODE_FUNC);
            $$->string =  (char*)malloc(sizeof(strlen($2->string)));
            strcpy($$->string, $2->string);
            addChild($$,$3);
            addChild($$,$5);
            addChild($$,$7);
        }
        | FUNCTION FunId Exp '=' Exp EndMark{
            yyerror( "function type definition not declared");
        }
        | DATATYPE ID '(' TypeList ')' EndMark{
            $$ = newNode(NODE_DATATYPE);
            struct nodeType *idname = newTokenNode(TOKE_ID);
            idname->string = malloc(sizeof(char)*strlen($2));
            addChild($$,idname);
            addChild($$,$4);
        }
        | Exp '=' Exp EndMark {
            $$ = newNode(NODE_ASSIGN);
            addChild($$,$1);
            addChild($$,$3);
        }
        | Exp EndMark {
            $$ = $1;
        }
        ;

FunId   : ID    {
            $$ = newTokenNode(TOKE_ID);
            $$->string = malloc(sizeof(char)*strlen($1));
        }
        | SpecialId{
            $$ = $1;
        }
        ;

EndMark : ';' 
        | '$'
        ;

FunTypeDef : TypeExp RARROW TypeExp{   
            $$ = newOpNode(OP_RARROW);
            addChild($$,$1); 
            addChild($$,$3);
        }
        ;

TypeExp : id {  
            switch($1->token) {
                case TOKE_INT:
                    $1->dataType.type = TYPEINT;
                    break;
                case TOKE_FLOAT:
                    $1->dataType.type = TYPEFLOAT;
                    break;
                case TOKE_BOOL:
                    $1->dataType.type = TYPEBOOL;
                    break;
                case TOKE_CHAR:
                    $1->dataType.type = TYPECHAR;
                    break;
                default:
                    yyerror("TypeExp error"); 
                    break;
            } 
            $$ = $1;  
        }
        | ID '(' TypeList ')' {
            $$ = newTokenNode(TOKE_ID);
            $$->string = (char*)malloc(sizeof(char)*strlen($1));
            strcpy($$->string, $1);
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
            //$2->nodeNum = NODE_TOKEN;
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
        if ($1->child->child == NULL) {
            $$->nodeNum = NODE_TUPLE_HEAD;
            $$ = $1->child;
        }
        */
        /*
        if ($1->nodeNum!=NODE_OP) {   
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
        struct nodeType *n1 = newNode(NODE_IFSTMT);
        struct nodeType *n3 = newNode(NODE_THENSTMT);
        struct nodeType *n5 = newNode(NODE_ELSESTMT);
        addChild($$, n1);
        addChild($$, n3);
        addChild($$, n5);
        addChild(n1,$2);
        addChild(n3,$4);
        addChild(n5,$6);
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
    }
    ;

ExpBind
    : Exp '=' Exp{
        $$ = newNode(NODE_ASSIGN);
        addChild($$, $1);
        addChild($$, $3);
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

OrOp: OR  { $$ = newOpNode(OP_OR);  }
    | NOR { $$ = newOpNode(OP_NOR);  }
    | XOR { $$ = newOpNode(OP_XOR);  } ;

AndExp
    : RelExp {$$=$1;}
    | AndExp AndOp RelExp {
        $$=$2;
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

AndOp
    : AND  {$$ = newOpNode(OP_AND);}
    | NAND {$$ = newOpNode(OP_NAND);}
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
    :  EQ {$$=newNode(NODE_OP); $$->op = OP_EQ;}
    | NE  {$$=newNode(NODE_OP); $$->op = OP_NE;}
    | '<' {$$=newNode(NODE_OP); $$->op = OP_LT;}
    | '>' {$$=newNode(NODE_OP); $$->op = OP_GT;}
    | LE  {$$=newNode(NODE_OP); $$->op = OP_LE;}
    | GE  {$$=newNode(NODE_OP); $$->op = OP_GE;}
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
    : '+'     {$$=newNode(NODE_OP); $$->op = OP_ADD;}
    | '-'     {$$=newNode(NODE_OP); $$->op = OP_SUB;}
    | PP      {$$=newNode(NODE_OP); $$->op = OP_PP;}
    | LARROW  {$$=newNode(NODE_OP); $$->op = OP_LARROW ;}
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
    : '*'    {$$=newOpNode(OP_MUL); } 
    | '/'    {$$=newOpNode(OP_DIV); } 
    | RARROW {$$=newOpNode(OP_RARROW); }  
    ;

ExpExp 
    : UnExp { $$ = $1;}
    | ExpExp '^' UnExp {
        $$ = newOpNode(OP_UPT); 
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

UnExp 
    : SubscriptExp {$$=$1;}
    | UnOp UnExp   {$$=$1; addChild($$,$2);}
    ;

UnOp
    : '#'{$$=newOpNode(OP_SHARP);}
    | '@'{$$=newOpNode(OP_AT);}
    | '-'{$$=newOpNode(OP_UMINUS);} 
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
    | '{' ApplyBody '}' {
        $$ = $2;
    }
    | '{' ApplyBody '|' Exp '}' {
        if ($2->nodeNum == NODE_APPLYBODY1) {
            $2->nodeNum = NODE_APPLYBODY3;
        }
        else {
            $2->nodeNum = NODE_APPLYBODY4;
        }
        $$ = $2;
        struct nodeType* filter = newNode(NODE_FILTER);
        addChild($$,filter);
        addChild(filter, $4);
    }
    | '[' ']' TypeExp {
        //Empty Sequence
        $$ = newNode(NODE_EMPSEQ);
        addChild($$, $3);
    }
    | '[' Exp SequenceTail ']'{
        if ($2->nodeNum!=NODE_TUPLE) {
        $$ = newNode(NODE_SEQ);
        addChild($$,$2);
        //addChild($$,$3);   
        }
        else{ 
            $$ =$2;
            $$->nodeNum=NODE_NEW_SEQ;
        }
        if ($3->nodeNum!=NODE_EMPTY)
          addChild($$,$3);   
    }
    | '(' Exp ')' {
        $$ = newNode(NODE_PAIR);
        addChild($$,$2);
        //$$ = $2;

    }
    | ID {
        $$ = newTokenNode(TOKE_ID);
        $$->string = (char*)malloc(sizeof(char)*strlen($1));
        strcpy($$->string, $1);
    }
    | id '(' Exp ')'{
            //FIXME float, int ... is also token ID
            // but different tokenType
        switch($1->token) {
         case TOKE_INT:
         case TOKE_BOOL:
         case TOKE_CHAR: 
            yyerror($1->string); 
         break;
         case TOKE_FLOAT:
            $1->token = TOKE_ID;
            $1->string = (char*)malloc(sizeof(char)*6);
            strcpy($1->string, "float");
         break;
         case TOKE_ID:
         break;
         default:
            yyerror("id error in id(exp)");
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
        $$ = newNode(NOTE_IMPLEMENT);
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
    
    }
    | RBind {
        $$ = newNode(NODE_RBINDS);
        addChild($$,$1);
    }
    ;
    
RBind
    : ID {
        $$ = newTokenNode(TOKE_ID);
        $$->string = (char*)malloc(sizeof(char)*strlen($1));
        strcpy($$->string, $1);
    }
    | Exp IN Exp{
        $$ = newNode(NODE_IN);
        addChild($$,$1);
        addChild($$,$3);
    }
    ;

id :    ID      {
                    $$ = newTokenNode(TOKE_ID);
                    $$->string = (char*)malloc(sizeof(char)*strlen($1));
                    strcpy($$->string, $1);
                } 
    |   T_INT   {$$ = newTokenNode(TOKE_INT);}
    |   T_FLOAT {$$ = newTokenNode(TOKE_FLOAT);}
    |   T_BOOL  {$$ = newTokenNode(TOKE_BOOL);}
    |   T_CHAR  {$$ = newTokenNode(TOKE_CHAR);}
    ;

SequenceTail
    : ':' Exp {
        $$ = $2;
        $$->nodeNum = NODE_SEQTAIL;
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
        $$ = newNode(NODE_INT);
        $$->iValue = $1;
    }
    | floatconst{
        $$ = newNode(NODE_FLOAT);
        $$->rValue = $1;
    }
    | boolconst{
        $$ = newNode(NODE_BOOL);
        $$->iValue = $1;
    }
    | stringconst{
        $$ = newNode(NODE_STRING);
        $$->string = malloc(sizeof(char)*strlen($1));
        strcpy($$->string, $1);
        $$->dataType.type = TYPESEQ;
        struct nodeType *c1 = newNode(NODE_TYPE_CHAR);
        c1->dataType.type = TYPECHAR;
        $$->dataType.child1 = &c1->dataType;

    }
    ;

%%

extern void removePair(struct nodeType *node);
extern void printNESL(struct nodeType *node, FILE* yyout);
extern void semanticPass(struct nodeType *node);

int ispfc=0;
int issqc=0;
int isomp=0;
int isrev=0;

struct nodeType *ASTRoot;
struct nodeType * newOpNode(enum OpType op) {
    struct nodeType *node = newNode(NODE_OP);
    node -> op = op;
    return node;
}

int yyerror(const char *s) {
    printf("Syntax error: %s \n",s);
    //printf("errror: %s",s);

    exit(0);
}

#define VERSION "0.0.1"
#define SQC_OUTPUT "output/%s.sqc.c" 
#define PFC_OUTPUT "output/%s.pfc.c"
#define OMP_OUTPUT "output/%s.omp.c"

static const char *help_manual = 
    "Usage:\n"
    "\t./NESL2C [-h] inputfile.nesl [--rev] {--pfc, --sqc, --omp} [-o outputfile.c] [--version]\n"
    "Optional arguments:\n"
    "\t-h, --help        show the help message and exit.\n"
    "\t--rev             for reverse output NESL.\n"
    "\t--pfc             for PartialFlattening C.\n"
    "\t--sqc             for Sequential C.\n"
    "\t--omp             for OpenMP C.\n"
    "\t-o outputfile.c   specific the output C file.\n"
    "\t-v, --version     show the program's version number and exit.\n";

static void Usage() {
    fprintf(stdout, "%s\n", help_manual);
    fflush(stdout);
}

static void Version() {
    fprintf(stdout, "NESL2C version %s\n", VERSION);
    fflush(stdout);
}

typedef enum {
    SQC,
    PFC,
    OMP,
    NONE
} CodeGenMode;

static struct option options[] = {
    {"rev", optional_argument, NULL, 'r'},
    {"sqc", optional_argument, NULL, 's'},
    {"pfc", optional_argument, NULL, 'p'},
    {"omp", optional_argument, NULL, 'm'},
    {"output", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'},
    {"version", no_argument, NULL, 'v'}
};

int main(int argc, char **argv) {
   
    CodeGenMode codeGenMode = NONE;
    int isRev = 0;   
    int c;
    char *input_file;
    char *output_file = NULL;
    while ((c = getopt_long(argc, argv, ":rspmhvo:", options, NULL)) != -1) {
        switch (c) {
            case 'r':
                isRev = 1;
                break;
            case 's':
                codeGenMode = SQC;
                break;
            case 'p':
                codeGenMode = PFC;
                break;
            case 'm':
                codeGenMode = OMP;
                break;
            case 'o':
                output_file = optarg;
                break;
            case 'v':
                Version();
                exit(0);
                break;
            case 'h':
            case '?':
                Usage();
                exit(1);
                break;
        }
    }
    
    if ((input_file = argv[optind]) == NULL || codeGenMode == NONE) {
        Usage();
        exit(1);
    }

    // TODO: check the input file extension (.nesl)
    //for ()

    // open nesl file 
    if ((yyin = fopen(input_file, "r")) == NULL) {
        fprintf(stderr, "Error: can not open the input file: %s\n", input_file);
        exit(1);
    }
    
    char *filename = basename(input_file);
    // deal with output file name
    if (output_file == NULL) {
        switch (codeGenMode) {
            case SQC:
                output_file = (char*)malloc(sizeof(char) * (strlen(SQC_OUTPUT) + strlen(filename)));    
                sprintf(output_file, SQC_OUTPUT, filename);
                break;
            case PFC:
                output_file = (char*)malloc(sizeof(char) * (strlen(PFC_OUTPUT) + strlen(filename)));    
                sprintf(output_file, PFC_OUTPUT, filename);
                break;
            case OMP:
                output_file = (char*)malloc(sizeof(char) * (strlen(OMP_OUTPUT) + strlen(filename)));    
                sprintf(output_file, OMP_OUTPUT, filename);
                break;
            case NONE:
            default:
                exit(1);
                break;
        }
    } else { 
        char *tmp = (char*)malloc(sizeof(char) * strlen(output_file));
        strcpy(tmp, output_file);
        output_file = tmp;
    }

    /**
    * Parse
    */ 
    assert(yyin);
    yyparse();
    fclose(yyin);
    printf("************************\n");
    printf("*** NO PARSING ERROR ***\n");
    printf("************************\n");

    /**
    * PrintTree
    */
    printTree(ASTRoot, 0);
    printf("************************\n");
    
    /**
    * Generate NESL to compare difference.
    */
    /* char *reveseNESL ; */
    /* reveseNESL = (char*)malloc(sizeof(char) * 100); */
    /* if (isrev) { */
        /* if (!usename) { */
            /* strcpy(reveseNESL,"reverseoutput/"); */
            /* strcat(reveseNESL, classname); */
            /* strcat(reveseNESL, ".nesl"); */
            /* printf("%s\n",reveseNESL); */
        /* } else { */
            /* sprintf(reveseNESL,"%s.nesl",classname); */
        /* } */
        /* yyout = fopen(reveseNESL,"w+"); */
        /* printNESL(ASTRoot, yyout);  */
        /* fclose(yyout); */
        /* printf("************************\n"); */
        /* printf("** REVERSE NESL DONE ***\n"); */
        /* printf("************************\n"); */
    /* } */
    /* free(reveseNESL); */
    
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
    yyout = fopen(output_file, "w+");
    
    // print Time information.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(yyout, "/**                             \n"
                   "* genereated by NESL2C from %s: \n"
                   "* GMT+8: %d-%d-%d %d:%d:%d\n"
                   "*/                            \n\n", 
                   filename, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
                   tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(yyout, DECHEAD);
    
    switch (codeGenMode) {
        case SQC: {
            //sqcheck(ASTRoot);
            pfcheck(ASTRoot);
        
            for (int i = 0; i < 100; i++) {
                strcpy(refTable.entries[i].name, "");
            }
            // generate the needed tuple structures
            gentuple(yyout);
            sqcodegen(yyout, ASTRoot);
            fprintf(yyout, "}\n\n"); // end of myFunc();
            fprintf(yyout, "int main() {\n");
            if (issrand)
                fprintf(yyout, "srand(time(0));\n");
            fprintf(yyout, ENDOFMAIN);
            break;
        }
      
        case PFC: {
            pfcheck(ASTRoot);
            for (int i = 0; i < 100; i++){
                strcpy(refTable.entries[i].name, "");
            }
            // generate the needed tuple structures
            gentuple(yyout);
            sqcodegen(yyout, ASTRoot); 
            fprintf(yyout, "}\n\n"); // end of myFunc();
            fprintf(yyout, "int main(){\n");
            if (issrand)
                fprintf(yyout, "srand(time(0));\n");
            fprintf(yyout, ENDOFMAIN);
            break;
        }   
      
        case OMP: 
            break;

        case NONE:
        default:
            exit(1);
            break;
    }

    fclose(yyout);
    free(output_file);
    
    printf("************************\n");
    printf("***  END OF NESL2C  ****\n");
    printf("************************\n");

    return 0;
}
