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
void removeNode(struct nodeType *node){
  struct nodeType *lhs = node->lsibling;
  struct nodeType *rhs = node->rsibling;
  struct nodeType *child = node->child;
  if(node->string != NULL)
    free(node);
  rhs->lsibling = NULL;
  lhs->rsibling = NULL;


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
            printf("%s%s\n", blank, node->string);
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
                case OP_EQ    : printf("%s==\n", blank); break;
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
                case OP_BIND  : printf("%s=\n", blank); break;
                case OP_PP  : printf("%s++\n", blank); break;
                
            }
            ident++;
            break;
        case NODE_STRING:
            printf("%sNODE_STRING: %s\n", blank, node->string);
            ident++;
            break;
        case NODE_RBINDS:
            printf("%sNODE_RBINDS\n", blank);
            ident++;
            break;
        case NODE_FreeVars:
            printf("%sNODE_FreeVars\n", blank);
            ident++;
            break;
        case NODE_SRCARR:
            printf("%sNODE_SRCARR\n", blank);
            ident++;
            break;
            
        case NODE_DEL:
            printf("%sNODE_DEL\n", blank);
            ident++;
            break;
        case NODE_TUPLE_HEAD:
            printf("%sNODE_TUPLE_HEAD\n", blank);
            ident++;
            break;

        case NODE_FUNC_TYPE:
            printf("%sTYPE_DEFS:\n", blank);
            ident++;
            break;
        case NODE_FUNC_INPUT:
            printf("%sINPUTS\n", blank);
            ident++;
            break;
        case NODE_FUNC_CALL:
            printf("%sNODE_FUNC_CALL\n", blank);
            ident++;
            break;
        case NODE_PAIR:
            printf("%sNODE_PAIR\n", blank);
            ident++;
            break;
        case NODE_TYPE_PAIR:
            printf("%sNODE_TYPE_PAIR\n", blank);
            ident++;
            break;
        case NODE_TYPE_SEQ:
            printf("%sTYPE_SEQ\n", blank);
            ident++;
            break;
        case NODE_SEQ_REF:
            printf("%sSEQ_REF\n", blank);
            ident++;
            break;
       
        case NODE_IN:
            printf("%sIN\n", blank);
            ident++;
            break;
        case NODE_FILTER:
            printf("%sFILTER\n", blank);
            ident++;
            break;
        case NODE_APPLYBODY1:
            printf("%sAPPLYBODY1\n", blank);
            ident++;
            break;
        case NODE_APPLYBODY2:
            printf("%sAPPLYBODY2\n", blank);
            ident++;
            break;
        case NODE_APPLYBODY3:
            printf("%sAPPLYBODY3\n", blank);
            ident++;
            break;
        case NODE_APPLYBODY4:
            printf("%sAPPLYBODY4\n", blank);
            ident++;
            break;
        case NODE_SEQTAIL:
            printf("%sSEQTAIL\n", blank);
            ident++;
            break;
        case NODE_LET:
            printf("%sEXP_LET\n", blank);
            ident++;
            break;
        case NODE_PATTERN:
            printf("%sNODE_PATTERN\n", blank);
            ident++;
            break;
        case NODE_SEQ_TUPLE:
            printf("%sNODE_SEQ_TUPLE\n", blank);
            ident++;
            break;
        case NODE_ACTION_TUPLE:
            printf("%sACTION_TUPLE\n", blank);
            ident++;
            break;
        case ELEM_TUPLE:
            printf("%sELEM_TUPLE\n", blank);
            ident++;
            break;
        case RB_TUPLE:
            printf("%sRB_TUPLE\n", blank);
            ident++;
            break;
        case NODE_TUPLE:
            printf("%sEXP_TUPLE\n", blank);
            ident++;
            break;
        case NODE_BIND:
            printf("%sNODE_BIND\n", blank);
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
        case NODE_BOOL:
            printf("%sNODE_BOOL: %s\n", blank, node->string);
            break;
        case NODE_INT:
            printf("%s%d\n", blank, node->iValue);
            break;
        case NODE_FLOAT:
            printf("%s%f\n", blank, node->rValue);
            break;
        case NODE_NEW_SEQ:
            printf("%sNODE_NEW_SEQ\n", blank);
            ident++;
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
        case NODE_FUNC:
            printf("%sFUNC: %s\n", blank, node->string);
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

