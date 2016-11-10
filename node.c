#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node.h"

struct nodeType* newNode(int type) {
    struct nodeType *node = (struct nodeType*)malloc(sizeof(struct nodeType));
    node->nodeType = type;
    node->valueValid = VALUE_INVALID;
    node->string = NULL;
    node->parent = NULL;
    node->child = NULL;
    node->lsibling = node;
    node->rsibling = node;

    return node;
}

void addChild(struct nodeType *node, struct nodeType *child) {
    child->parent = node;

    if(node->child == NULL) {
        node->child = child;
    }
    else {
        child->lsibling = node->child->lsibling;
        child->rsibling = node->child;
        node->child->lsibling->rsibling = child;
        node->child->lsibling = child;
    }
}

void deleteNode(struct nodeType *node) {
    if(node->string != NULL)
        free(node->string);
    free(node);
}

void printTree(struct nodeType *node, int ident) {
    static char blank[1024];
    for(int i=0; i<ident; i++)
        blank[i] = ' ';
    blank[ident] = 0;

    switch(node->nodeType) {
        case NODE_TOKEN:
            printf("%sToken %s\n", blank, node->string);
            ident++;
            break;
        case NODE_OP:
            switch(node->op) {
                case OP_ADD   : printf("%s+\n", blank); break;
                case OP_SUB   : printf("%s-\n", blank); break;
                case OP_MUL   : printf("%s*\n", blank); break;
                case OP_DIV   : printf("%s/\n", blank); break;
                case OP_GT    : printf("%s>\n", blank); break;
                case OP_LT    : printf("%s<\n", blank); break;
                case OP_EQ    : printf("%s=\n", blank); break;
                case OP_GE    : printf("%s>=\n", blank); break;
                case OP_LE    : printf("%s<=\n", blank); break;
                case OP_NE    : printf("%s/= (not equal)\n", blank); break;
                case OP_NOT   : printf("%sNOT\n", blank); break;
                case OP_OR    : printf("%sOR\n", blank); break;
                case OP_COMMA : printf("%s,\n", blank); break;
                case OP_AND   : printf("%sAND\n", blank); break;
                case OP_NOR   : printf("%sNOR\n", blank); break;
                case OP_NAND  : printf("%sNAND\n", blank); break;
                case OP_XOR   : printf("%sXOR\n", blank); break;
                case OP_LARROW: printf("%s<-\n", blank); break;
                case OP_RARROW: printf("%s->\n", blank); break;
                case OP_UPT   : printf("%s^\n", blank); break;
                case OP_SHARP : printf("%s#\n", blank); break;
                case OP_AT    : printf("%s@\n", blank); break;
                case OP_UMINUS: printf("%sUMINUS - \n", blank); break;
                
            }
            ident++;
            break;
       
        case NODE_APPEACH :
            printf("%sApply to each\n", blank);
            ident++;
            break;
        case NODE_IFELSE  :
            printf("%sIFELSE\n", blank);
            ident++;
            break;
        case NODE_IFSTMT  :
            printf("%sIF\n", blank);
            ident++;
            break;
        case NODE_THENSTMT:
            printf("%sTHEN\n", blank);
            ident++;
            break;
        case NODE_ELSESTMT:
            printf("%sELSE\n", blank);
            ident++;
            break;
        
        case NODE_TYPEEXP:
            printf("%sNODE_TYPEEXP\n", blank);
            ident++;
            break;
        case NODE_INFER:
            printf("%sNODE_INFER\n", blank);
            ident++;
            break;
        case NODE_NESL:
            printf("%sNODE_NESL\n", blank);
            ident++;
            break;
        case NODE_EXP:
            printf("%sNODE_EXP\n", blank);
            ident++;
            break;
        case NODE_INT:
            printf("%s%d\n", blank, node->iValue);
            break;
        case NODE_FLOAT:
            printf("%s%f\n", blank, node->rValue);
            break;
        case NODE_VAR_OR_PROC:
            printf("%s%s\n", blank, node->string);
            break;
        case NODE_SEQ:
            printf("%sNODE_SEQ\n", blank);
            ident++;
            break;
        case NODE_LIST:
            printf("%sLIST\n", blank);
            ident++;
            break;
        case NODE_ERROR:
            printf("%sERROR\n", blank);
            break;
        case NODE_TYPE_ARRAY:
            printf("%sARRAY\n", blank);
            ident++;
            break;
        case NODE_ARR_REF:
            printf("%sARRAY_REF\n", blank);
            ident++;
            break;
        case NODE_FUNC:
            printf("%sFUNC\n", blank);
            ident++;
            break;
        case NODE_PROC:
            printf("%sPROC\n", blank);
            ident++;
            break;
        case NODE_PROC_AND_FUNC_DECL:
            printf("%sP&FUNC_DECL\n", blank);
            ident++;
            break;
        case NODE_CMP_STMT:
            printf("%sCOMP_STMT\n", blank);
            ident++;
            break;
        case NODE_TYPE_INT:
            printf("%sTYPE_INT\n", blank);
            ident++;
            break;
        case NODE_TYPE_FLOAT:
            printf("%sTYPE_REAL\n", blank);
            ident++;
            break;
        case NODE_ASSIGN_STMT:
            printf("%sASSIGN_STMT\n", blank);
            ident++;
            break;
        case NODE_SYM_REF:
            printf("%sSYM_REF %s\n", blank, node->string);
            ident++;
            break;
        default:
            printf("%sdefault:%d\n", blank, node->nodeType);
          break;
    }

    struct nodeType *child = node->child;
    if(child != NULL) {
        do {
            printTree(child, ident);
            child = child->rsibling;
        } while(child != node->child);
    }
}

