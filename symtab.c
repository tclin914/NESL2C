#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n",s);}
struct SymTable SymbolTable;

struct SymTableEntry* findSymbol(char *s) {
    for(int i=0; i<SymbolTable.size; i++) {
        if(strcmp(s, SymbolTable.entries[i].name) == 0) {
            return &SymbolTable.entries[i];
        }
    }

    return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link) {
    printf("s:%s, Type:%d\n",s, type);
    if(findSymbol(s) != 0) {
        printf("Error: duplicate declaration of variable %s\n", s);
        exit(0);
    }

    int index = SymbolTable.size;
    SymbolTable.size++;

    strcpy(SymbolTable.entries[index].name, s);
    SymbolTable.entries[index].type = type;
    SymbolTable.entries[index].link = link;
    
    return &SymbolTable.entries[index];
}

struct nodeType* nthChild(int n, struct nodeType *node) {
    struct nodeType *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}

void semanticCheck(struct nodeType *node) {
 //   printf("nodetype:%d\n", node->nodeType);
    switch(node->nodeType) {

        // Declaration part, add to symbol table.
        case NODE_VAR_DECL: {

            struct nodeType *typeNode = nthChild(1, node);
            enum StdType valueType;
            
            do{
              
              switch(typeNode->nodeType){
              case NODE_TYPE_INT:
                valueType = TypeInt;
                break;
              case NODE_TYPE_REAL:
                valueType = TypeReal;
                break;
              case NODE_TYPE_ARRAY:
                valueType = TypeArray;
                break;
              }

              struct nodeType *idList = nthChild(1, typeNode);
              struct nodeType *arrType = typeNode->child;
              struct nodeType *idNode = idList->child;
              int arraycount=1;

              /* For ArrayType Declaration. */
              if(valueType == TypeArray){

                while (arrType->nodeType==NODE_TYPE_ARRAY){
                  // Traverse to find the declared type of arrays.
                  arrType = arrType->child;
                  arraycount ++;
                }
                
                while(idList->nodeType != NODE_LIST){
                  // Find the TokenList.
                  idList = idList->rsibling;
                }
                idNode = idList->child;
                
                // Add all token under the idList.
                do {
                  if(!findSymbol(idNode->string)){
                    addVariable(idNode->string, arrType->valueType, typeNode);
                    typeNode->arraydepth = arraycount;
                    printf("%s: arraydepth: %d\n",idNode->string, arraycount);
                  }
                  idNode = idNode->rsibling;
                  // FIXME Should happen infinite loop here when only one child. 
                  // BUT it didn't happen ?!
                } while(idNode != idList->child);

              }
              /* For Int and Real declaration. */
              else{
                do {
                  addVariable(idNode->string, valueType, typeNode);
                  idNode = idNode->rsibling;
                } while(idNode != idList->child);
              }
              typeNode = typeNode -> rsibling;

            }while(typeNode != node->child);
            return;
        }
        /** End of Declaration. **/
        case NODE_PROC_AND_FUNC_DECL:{
          //TODO
          struct nodeType * pfNode = nthChild(1, node);
          /*switch(pfNode->nodeType){
            
          }*/

          return;
        }
          
        /* General cases in the Compoud statement. 
           Focus on array stuff.
        */

        /* This case is simplified, actually you should check
           the symbol is a variable or a function with no parameter */
        case NODE_VAR_OR_PROC: 
        case NODE_TOKEN:
        case NODE_SYM_REF: {
            struct SymTableEntry *entry = findSymbol(node->string);
            if(entry == 0) {
                printf("Error: undeclared variable %s\n", node->string);
                exit(0);
            }

            node->entry = entry;
            node->valueType = entry->type;
            node->ref = entry->link;
            return;
        }
        
        case NODE_ARR_REF: {

          struct nodeType *child1 = nthChild(1, node);
          struct nodeType *child2 = nthChild(2, node);

          semanticCheck(child1);
          semanticCheck(child2);
          
          if(child2->valueType == TypeInt){
            if(child1->ref->idxstart > child2->iValue ||
               child1->ref->idxend < child2->iValue){
              printf("Index out of bound.\n");
            }
          }
          else if(child2->valueType == TypeReal){
              printf("invalid index type: float\n");
              exit(0);
          }
          //printf("child2->valueType:%d\n",child2->valueType);
          node->valueType = child1->valueType;
          node->ref = child1->ref->child;
          if(child1->nodeType == NODE_SYM_REF){
            node->arraydepth = child1->ref->arraydepth-1;
          }
          else {
            node->arraydepth = child1->arraydepth - 1;
          }
          //printf("nodedepth:%d\n",node->arraydepth);
          return;
        }
/*
            struct nodeType *child1, *child2;
            struct nodeType *arrnode=NULL;
            struct nodeType *arrtype = nthChild(1,node);
            struct nodeType *parent=NULL;
            struct SymTableEntry* arrayEnt=NULL;
            
            // Find arrayID
            while(arrtype->nodeType == NODE_ARR_REF){
              arrtype = arrtype->child;
            } 
            // Get array node. update "node->valueType".
            arrayEnt = findSymbol(arrtype->string);
            arrnode = arrayEnt->link;
            node->valueType = arrayEnt->type;
            
            // check the indexes from bottom(arrtype) to up(node).
            parent = arrtype->parent->parent;
            child2 = arrtype->rsibling;
//                semanticCheck(child2);
            do{
              
              if( child2->nodeType != NODE_INT)
                // check the rsb and update it's valueType and iValue.
              
              if(child2->valueType!=TypeInt){
                printf("index not Integer\n");
                exit(0);
              }

              else if( (child2->iValue > arrnode->idxend) ||
                       (child2->iValue < arrnode->idxstart) ){
                printf("array %s index %d out of boundery %d..%d .\n",
                       arrtype->string, child2->iValue, arrnode->idxstart,
                       arrnode->idxend);                      
              //  exit(0);
              }

              arrnode = arrnode->child;
              parent = parent->parent;
              child2 = parent->rsibling;
 //               semanticCheck(child2);

              if(arrnode ==NULL) {
                printf("too much references.\n");
                exit(0);
              }


            }while (parent != node);
            

            return;
        }
*/
        case NODE_TYPE_ARRAY: {
            // won't happen.
            node->valueType = TypeArray;
            printf("yo\n");
            return;
        }
        case NODE_INT: {
            node->valueType = TypeInt;
            return;
        }

        case NODE_REAL: {
            node->valueType = TypeReal;
            return;
        }

        /* Only implemented binary op here, you should implement unary op */
        case NODE_OP:{
            //TODO check the both side have the same type.
            struct nodeType *child1 = nthChild(1,node);
            struct nodeType *child2 = nthChild(2,node);
            semanticCheck(child1);
            semanticCheck(child2);
            if(child1->valueType == child2->valueType){
            node->valueType = child1->valueType;
/*
            switch( node->op){
              case OP_ADD:
                if(node->valueType == TypeInt)
                  node->iValue = child1->iValue + child2->iValue;
                else 
                  node->rValue = (float)child1->rValue + (float)child2->rValue;
                break;
              case OP_SUB:
                if(node->valueType == TypeInt)
                  node->iValue = child1->iValue - child2->iValue;
                else 
                  node->rValue = (float)child1->rValue - (float)child2->rValue;
                break;
              case OP_MUL:
                if(node->valueType == TypeInt)
                  node->iValue = child1->iValue * child2->iValue;
                else 
                  node->rValue = (float)child1->rValue * (float)child2->rValue;
                break;
              case OP_DIV:
                if(node->valueType == TypeInt)
                  node->iValue = child1->iValue / child2->iValue;
                else 
                  node->rValue = (float)child1->rValue / (float)child2->rValue;
                break;
              case OP_GT :
                break;
              case OP_LT :
                break;
              case OP_EQ :
                break;
              case OP_GE :
                break;
              case OP_LE :
                break;
              case OP_NE :
                break;
              case OP_NOT:
                break;
            }
            */
            return; 
            }
            else {
              printf("OP type dismatch\n");
              exit(0);
            }
            }
        /* You should check the LHS of assign stmt is assignable
           You should also report error if LHS is a function with no parameter 
           (function is not implemented in this sample, you should implement it) */ 
        case NODE_ASSIGN_STMT: {
            struct nodeType *child1 = nthChild(1, node);
            struct nodeType *child2 = nthChild(2, node);
            semanticCheck(child1);
            semanticCheck(child2);

            /* We only implement the checking for integer and real types
               you should implement the checking for array type by yourself */
            if(child1->valueType != child2->valueType) {
                if(node->nodeType == NODE_OP)
                    printf("Error: type mismatch for operator\n");
                else{
                    printf("Error: type mismatch for assignment\n");
                    printf("type: %d, %d\n",child1->valueType,child2->valueType);
                    }
                exit(0);
            }
            if(child1->nodeType==NODE_ARR_REF){
              if(child2->nodeType==NODE_ARR_REF){
                if(child2->arraydepth != child1->arraydepth)
                  printf("array depth dismatch. child1:%d, child2:%d\n",
                  child1->arraydepth, child2->arraydepth);
              }
              else{
                if(child1->arraydepth != 0)
                  printf("LHS is array.\n");   
              }
              
            }


            node->valueType = child1->valueType;

            return;
        }
    }

    /* Default action for other nodes not listed in the switch-case */
    struct nodeType *child = node->child;
    if(child != 0) {
        do {
            semanticCheck(child);
            child = child->rsibling;
        } while(child != node->child);
    }
}

