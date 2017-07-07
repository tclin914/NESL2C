#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <getopt.h>
#include <libgen.h>

#include "assert.h"

#define VERSION "0.0.1"
#define SQC_OUTPUT "output/%s.sqc.c" 
#define PFC_OUTPUT "output/%s.pfc.c"
#define OMP_OUTPUT "output/%s.omp.c"

#define DECHEAD "#include <stdio.h>\n#include <stdlib.h>\n#include \"sqmacro.h\"\n"
#define ENDOFMAIN "SET_HEAP_SIZE(MALLOC_HEAP_SIZE);\nmyFunc1();\ncheckglobal();\nCUDA_ERROR_CHECK();\nreturn 1;\n}\n"

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

int yyparse();
extern FILE *yyin;

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
    /* printTree(ASTRoot, 0); */
    /* printf("************************\n"); */
    
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
        /* cout = fopen(reveseNESL,"w+"); */
        /* printNESL(ASTRoot, cout);  */
        /* fclose(cout); */
        /* printf("************************\n"); */
        /* printf("** REVERSE NESL DONE ***\n"); */
        /* printf("************************\n"); */
    /* } */
    /* free(reveseNESL); */
    
    /**
    * Semantic Check: type
    */
    // TODO 
    /* semanticPass(ASTRoot); */
     
    printf("************************\n");
    printf("** NO SEMANTIC ERROR ***\n");
    printf("************************\n");

    /**
    * Generate C file.
    */
    FILE *cout = fopen(output_file, "w+");
    
    // print Time information.
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    fprintf(cout, "/**                             \n"
                   "* genereated by NESL2C from %s: \n"
                   "* GMT+8: %d-%d-%d %d:%d:%d\n"
                   "*/                            \n\n", 
                   filename, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, 
                   tm.tm_hour, tm.tm_min, tm.tm_sec);
    fprintf(cout, DECHEAD);
    
    switch (codeGenMode) {
        case SQC: {
            /* //sqcheck(ASTRoot); */
            /* pfcheck(ASTRoot); */
        
            /* for (int i = 0; i < 100; i++) { */
                /* strcpy(refTable.entries[i].name, ""); */
            /* } */
            /* // generate the needed tuple structures */
            /* gentuple(cout); */
            /* sqcodegen(cout, ASTRoot); */
            /* fprintf(cout, "}\n\n"); // end of myFunc(); */
            /* fprintf(cout, "int main() {\n"); */
            /* if (issrand) */
                /* fprintf(cout, "srand(time(0));\n"); */
            /* fprintf(cout, ENDOFMAIN); */
            /* break; */
        }
      
        case PFC: {
            /* pfcheck(ASTRoot); */
            /* for (int i = 0; i < 100; i++){ */
                /* strcpy(refTable.entries[i].name, ""); */
            /* } */
            /* // generate the needed tuple structures */
            /* gentuple(cout); */
            /* sqcodegen(cout, ASTRoot);  */
            /* fprintf(cout, "}\n\n"); // end of myFunc(); */
            /* fprintf(cout, "int main(){\n"); */
            /* if (issrand) */
                /* fprintf(cout, "srand(time(0));\n"); */
            /* fprintf(cout, ENDOFMAIN); */
            break;
        }   
      
        case OMP: 
            break;

        case NONE:
        default:
            exit(1);
            break;
    }

    fclose(cout);
    free(output_file);
    
    printf("************************\n");
    printf("***  END OF NESL2C  ****\n");
    printf("************************\n");

    return 0;
}
