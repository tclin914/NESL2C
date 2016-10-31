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
                case OP_ADD:
                    printf("%s+\n", blank);
                    break;
                case OP_SUB:
                    printf("%s-\n", blank);
                    break;
                case OP_MUL:
                    printf("%s*\n", blank);
                    break;
                case OP_DIV:
                    printf("%s/\n", blank);
                    break;
                case OP_GT:
                    printf("%s>\n", blank);
                    break;
                case OP_LT:
                    printf("%s<\n", blank);
                    break;
                case OP_EQ:
                    printf("%s=\n", blank);
                    break;
                case OP_GE:
                    printf("%s>=\n", blank);
                    break;
                case OP_LE:
                    printf("%s<=\n", blank);
                    break;
                case OP_NE:
                    printf("%s!=\n", blank);
                    break;
            }
            ident++;
            break;
        case NODE_INT:
            printf("%s%d\n", blank, node->iValue);
            break;
        case NODE_REAL:
            printf("%s%f\n", blank, node->rValue);
            break;
        case NODE_VAR_OR_PROC:
            printf("%s%s\n", blank, node->string);
            break;
        case NODE_CALLPROC:
            printf("%s%s\n", blank, node->string);
            ident++;
            break; 
        case NODE_TAIL:
            printf("%sTAIL\n", blank);
            ident++;
            break;
        case NODE_PLIST:
            printf("%sPLIST\n", blank);
            ident++;
            break;
        case NODE_LIST:
            printf("%sLIST\n", blank);
            ident++;
            break;
        case NODE_WHILE:
            printf("%sWHILE\n", blank);
            ident++;
            break;
        case NODE_IFSTMT:
            printf("%sIFELSE\n", blank);
            ident++;
            break;
        case NODE_ERROR:
            printf("%sERROR\n", blank);
            break;
        case NODE_TYPE_ARRAY:
            printf("%sARRAY\n", blank);
            printf("%s start:%d\n",blank, node->idxstart);
            printf("%s end:%d\n",blank, node->idxend);
            ident++;
            break;
        case NODE_ARR_REF:
            printf("%sARRAY_REF\n", blank);
            ident++;
            break;
        case NODE_PROGRAM:
            printf("%sPROGRAM\n", blank);
            ident++;
            break;
        case NODE_SUB_HEAD:
            printf("%sHEAD\n", blank);
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
        case NODE_VAR_DECL:
            printf("%sVAR_DECL\n", blank);
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
        case NODE_TYPE_REAL:
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

