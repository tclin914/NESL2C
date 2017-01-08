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
    printf("s:%s, valueType:%d\n",s, type);
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

void looptogether(struct nodeType *node1, struct nodeType *node2){
  //TODO
  if(node1->nodeType == NODE_TUPLE&&node2->nodeType == NODE_TUPLE){
    countTuple(node1);
    countTuple(node2);
    if(node1->tuplecount == node2->tuplecount){
      if(node1->child!=0&&node2->child!=0){
        looptogether(node1->child, node2->child);
      }
    }
  
  }
}

void countTuple(struct nodeType *node){
  if(node->nodeType == NODE_TUPLE){
    struct nodeType *child = node->child;
    int count=0;
    int tuplechildcount=0;
    if(child!=0){
      do{
        count++;
        if(child->nodeType == NODE_TUPLE){
          countTuple(child);
          tuplechildcount++;
        }
        child = child->rsibling;
      }while(child!=node->child);
    }
    node->tuplecount = count;

  }
  

}


void typeCheck(struct nodeType *node){
 switch(node->nodeType){
 case NODE_LIST:{
   struct nodeType *child = node->child;
   if(child!=0){
     do{
       typeCheck(child);
       child = child->rsibling;
     }while( child!=node->child);
   }
   break;
 }
 case NODE_FUNC:{
   struct nodeType *child = node->child;
   
   //TODO 
   // FIXME change the yacc.y stuff , parse typedeclare the same way as usual.
   looptogether(child->child, child->rsibling->child->child);

   // input param
   typeCheck(child);
  
   // input type
   typeCheck(child->rsibling);
   
   // function body.
   typeCheck(child->rsibling->rsibling);
   break;
 }
 case NODE_TUPLE:{
   struct nodeType *child = node->child;
   typeCheck(child);
   child = child->rsibling;
   while(child!=node->child){
     typeCheck(child);
     child = child->rsibling;
   }
   break;
 }
 case NODE_TYPE_PAIR:{
   struct nodeType *child = node->child;

   typeCheck(child);
   child = child->rsibling;
   while(child!=node->child){
     typeCheck(child);
     child = child->rsibling;
   }
   break;
 }
 case NODE_OP:{
   switch(node->op){
   case OP_BIND:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_ADD:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_SUB:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_MUL:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_DIV:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_RARROW:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_LT:
     typeCheck(node->child);
     typeCheck(node->child->rsibling);
     break;
   case OP_AT:
     typeCheck(node->child);
     break;
   case OP_UMINUS:
     typeCheck(node->child);
     break;
   case OP_SHARP:
     typeCheck(node->child);
     break;

   case OP_GT:     
    typeCheck(node->child); 
    typeCheck(node->child->rsibling);  
    break;
   case OP_EQ:
    typeCheck(node->child);
    typeCheck(node->child->rsibling);  
    break;
   case OP_GE:    
    typeCheck(node->child); 
    typeCheck(node->child->rsibling);  break;
   case OP_LE:     
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  break;
   case OP_NE:     
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  break;
   case OP_NOT:    
   typeCheck(node->child); 
   typeCheck(node->child->rsibling); break;
   case OP_OR:     
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  
   break;
   case OP_COMMA:  
   typeCheck(node->child); 
   typeCheck(node->child->rsibling); break;
   case OP_AND:    
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  break;
   case OP_NOR:    
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  break;
   case OP_NAND:   
   typeCheck(node->child); 
   typeCheck(node->child->rsibling); break;
   case OP_XOR:    
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  break;
   case OP_LARROW: 
   typeCheck(node->child); 
   typeCheck(node->child->rsibling);  break;
   case OP_UPT:    
   typeCheck(node->child);
   typeCheck(node->child->rsibling);  break;
   case OP_PP:     
   typeCheck(node->child); 
   typeCheck(node->child->rsibling); break;
   }
   break;
 }
 case NODE_TOKEN:{
   switch(node->tokenType){
   case TOKE_ID:
     //addVariable(
     break;
   case TOKE_INT:
     node->valueType = TypeInt;
     break;
   case TOKE_FLOAT:
     node->valueType = TypeFloat;
     break;
   }
   break;
 }
 case NODE_LET:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);
   break; 
 } 
 case NODE_PATTERN:
 case NODE_EXP:{
   struct nodeType *child = node->child;
   if(child!=0){
     do{
       typeCheck(child);
       child = child->rsibling;
     }while(child!=node->child);
   }
   break;
 }
 case NODE_BIND:{
   struct nodeType *child = node->child;
   if(child!=0){
     do{
       typeCheck(child);
       child = child->rsibling;
     }while(child!=node->child);
   }
   break;
 }
 case NODE_FUNC_CALL:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);

   break;
 }
 case NODE_FLOAT:{
   node->valueType = TypeFloat; 
   break;
 }
 case NODE_INT:{
   break;
 }

 case NODE_IN:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);
   break;
 }
 case NODE_APPLYBODY1:{
   typeCheck(node->child);
   break;
 }
 case NODE_APPLYBODY2:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);

   break;
 }
 case NODE_APPLYBODY3:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);

   break;
 }
 case NODE_APPLYBODY4:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);
   typeCheck(node->child->rsibling->rsibling);

   break;
 }
 case NODE_TYPE_SEQ:
 case NODE_SEQ:{
   struct nodeType *child = node->child;
   if(child!=0){
     typeCheck(child);
     child = child->rsibling;
     while(child!=node->child){
       typeCheck(child);
       child = child->rsibling;
     }
   }
   break;
 }
 case NODE_IFELSE:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);
   typeCheck(node->child->rsibling->rsibling);
   break;
 }
 case NODE_IFSTMT:{
   typeCheck(node->child);
   break;
 }
 case NODE_THENSTMT:{
   typeCheck(node->child);
   break;
 }
 case NODE_ELSESTMT:{
   typeCheck(node->child);
   break;
 }
 case NODE_RBINDS:{
   struct nodeType * child = node->child;
   if(child!=0){
     typeCheck(child);
     child=child->rsibling;
     while(child!=node->child){
       typeCheck(child);

       child = child->rsibling;
     }
   }
   break;
 }
 case NODE_SEQ_REF:{
   typeCheck(node->child);
   typeCheck(node->child->rsibling);
   break;
 }
 case NODE_FILTER:{
   typeCheck(node->child);
   break;
 }
 case NODE_STRING:{
   node->valueType = NODE_TYPE_CHAR;
   break;
 }
 
 }// End of Switch

}// End of typeCheck

void printNESL(struct nodeType *node, FILE* yyout){
  switch(node->nodeType){
    case NODE_LIST:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child, yyout);
          fprintf(yyout, "\n");
          child = child->rsibling;
        }while( child!=node->child);
      }
      break;
    }
    case NODE_FUNC:{
      struct nodeType *child = node->child;
      fprintf(yyout,"function %s",node->string);
      printNESL(child, yyout);
      fprintf(yyout,":\n");
      printNESL(child->rsibling, yyout);
      fprintf(yyout,"=\n");
      printNESL(child->rsibling->rsibling, yyout);
      fprintf(yyout,";\n");
      break;
    }
    case NODE_TUPLE:{
     struct nodeType *child = node->child;
      printNESL(child, yyout);
      child = child->rsibling;
      while(child!=node->child){
        fprintf(yyout,",");
        printNESL(child, yyout);
        child = child->rsibling;
      }
      break;
    }
    case NODE_TYPE_PAIR:{
      struct nodeType *child = node->child;

      fprintf(yyout,"(");
      printNESL(child, yyout);
      child = child->rsibling;
      while(child!=node->child){
        fprintf(yyout,",");
        printNESL(child, yyout);
        child = child->rsibling;
      }
      fprintf(yyout,")");
      break;
    }
    case NODE_OP:{
      switch(node->op){
        case OP_BIND:
          printNESL(node->child, yyout);
          fprintf(yyout,"=");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_ADD:
          printNESL(node->child, yyout);
          fprintf(yyout," + ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_SUB:
          printNESL(node->child, yyout);
          fprintf(yyout,"-");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_MUL:
          printNESL(node->child, yyout);
          fprintf(yyout,"*");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_DIV:
          printNESL(node->child, yyout);
          fprintf(yyout,"/");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_RARROW:
          printNESL(node->child, yyout);
          fprintf(yyout,"->");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_LT:
          printNESL(node->child, yyout);
          fprintf(yyout,"<");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_AT:
          fprintf(yyout,"@");
          printNESL(node->child, yyout);
          break;
        case OP_UMINUS:
          fprintf(yyout,"-");
          printNESL(node->child, yyout);
          break;
        case OP_SHARP:
          fprintf(yyout,"#");
          printNESL(node->child, yyout);
          break;
        
        case OP_GT:     printNESL(node->child, yyout); fprintf(yyout,">"); printNESL(node->child->rsibling, yyout);  break;
        case OP_EQ:     printNESL(node->child, yyout); fprintf(yyout," == "); printNESL(node->child->rsibling, yyout);  break;
        case OP_GE:     printNESL(node->child, yyout); fprintf(yyout," >= "); printNESL(node->child->rsibling, yyout);  break;
        case OP_LE:     printNESL(node->child, yyout); fprintf(yyout," <= "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NE:     printNESL(node->child, yyout); fprintf(yyout," != "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NOT:    printNESL(node->child, yyout); fprintf(yyout," NOT "); printNESL(node->child->rsibling, yyout); break;
        case OP_OR:     printNESL(node->child, yyout); fprintf(yyout," OR "); printNESL(node->child->rsibling, yyout);  break;
        case OP_COMMA:  printNESL(node->child, yyout); fprintf(yyout," , "); printNESL(node->child->rsibling, yyout); break;
        case OP_AND:    printNESL(node->child, yyout); fprintf(yyout," AND "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NOR:    printNESL(node->child, yyout); fprintf(yyout," NOR "); printNESL(node->child->rsibling, yyout);  break;
        case OP_NAND:   printNESL(node->child, yyout); fprintf(yyout," NAND "); printNESL(node->child->rsibling, yyout); break;
        case OP_XOR:    printNESL(node->child, yyout); fprintf(yyout," XOR "); printNESL(node->child->rsibling, yyout);  break;
        case OP_LARROW: printNESL(node->child, yyout); fprintf(yyout," <- "); printNESL(node->child->rsibling, yyout);  break;
        case OP_UPT:    printNESL(node->child, yyout); fprintf(yyout," ^ "); printNESL(node->child->rsibling, yyout);  break;
        case OP_PP:     printNESL(node->child, yyout); fprintf(yyout," ++ "); printNESL(node->child->rsibling, yyout); break;




      }
      break;
    }
    case NODE_TOKEN:{
      switch(node->tokenType){
        case TOKE_ID:
          fprintf(yyout,"%s",node->string);
          break;
        case TOKE_INT:
          fprintf(yyout,"INT");
          break;
        case TOKE_FLOAT:
          fprintf(yyout,"FLOAT");
          break;
        
      
        
      }
      break;
    }
    case NODE_LET:{
      fprintf(yyout,"let ");
      printNESL(node->child, yyout);
      fprintf(yyout,"in ");
      printNESL(node->child->rsibling, yyout);
      //fprintf(yyout,";\n");
      break; 
    } 
    case NODE_PATTERN:
    case NODE_EXP:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child, yyout);
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
    case NODE_BIND:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          printNESL(child, yyout);
          fprintf(yyout,";\n");
          child = child->rsibling;
        }while(child!=node->child);
      }
      break;
    }
    case NODE_FUNC_CALL:{
      printNESL(node->child, yyout);
      //fprintf(yyout,"(");
      printNESL(node->child->rsibling, yyout);
      //fprintf(yyout,")");

      break;
    }
    case NODE_FLOAT:{
      fprintf(yyout,"%f",node->rValue);
      break;
    }
    case NODE_INT:{
      fprintf(yyout,"%d",node->iValue);
      break;
    }

    case NODE_IN:{
      printNESL(node->child, yyout);
      fprintf(yyout," IN ");
      printNESL(node->child->rsibling, yyout);
      break;
    }
    case NODE_APPLYBODY1:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout,"}");
      break;
    }
    case NODE_APPLYBODY2:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout," : ");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout,"}");
      
      break;
    }
    case NODE_APPLYBODY3:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout," | ");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout,"}");
      
      break;
    }
    case NODE_APPLYBODY4:{
      fprintf(yyout,"{ ");
      printNESL(node->child, yyout);
      fprintf(yyout," : ");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout," | ");
      printNESL(node->child->rsibling->rsibling, yyout);
      fprintf(yyout,"}");
      
      break;
    }
    case NODE_TYPE_SEQ:
    case NODE_SEQ:{
      struct nodeType *child = node->child;
      fprintf(yyout,"[");
      if(child!=0){
        printNESL(child, yyout);
        child = child->rsibling;
        while(child!=node->child){
          fprintf(yyout,", ");
          printNESL(child, yyout);
          child = child->rsibling;
        }
      }
      fprintf(yyout,"]");
      break;
    }
    case NODE_IFELSE:{
      printNESL(node->child, yyout);
      printNESL(node->child->rsibling, yyout);
      printNESL(node->child->rsibling->rsibling, yyout);
      break;
    }
    case NODE_IFSTMT:{
      fprintf(yyout,"if ");
      printNESL(node->child, yyout);
      break;
    }
    case NODE_THENSTMT:{
      fprintf(yyout,"then ");
      printNESL(node->child, yyout);
      fprintf(yyout,"\n");
      break;
    }
    case NODE_ELSESTMT:{
      fprintf(yyout,"else \n");
      printNESL(node->child, yyout);
      break;
    }
    case NODE_RBINDS:{
      struct nodeType * child = node->child;
      if(child!=0){
        printNESL(child, yyout);
        child=child->rsibling;
        while(child!=node->child){
          fprintf(yyout,";");
          printNESL(child, yyout);

          child = child->rsibling;
        }
      }
      break;
    }
    case NODE_SEQ_REF:{
      printNESL(node->child, yyout);
      fprintf(yyout,"[");
      printNESL(node->child->rsibling, yyout);
      fprintf(yyout,"]");
      break;
    }
    case NODE_FILTER:{
      printNESL(node->child, yyout);
      break;
    }
    case NODE_STRING:{
      fprintf(yyout,"%s", node->string);
      break;
    }

    
  
  }// End of Switch

}
void semanticCheck(struct nodeType *node) {
 //   printf("nodetype:%d\n", node->nodeType);
    switch(node->nodeType){
      case NODE_FUNC:
      {
        
        if(!findSymbol(node->string)){
          int vtype = node->child->rsibling->child->child->rsibling->valueType;
          addVariable(node->string, vtype, node);
        }
        break;
      }

      case NODE_PATTERN:
      {
        
        struct nodeType *child = node->child;
        struct nodeType *idNode = child;
        do{
          idNode=child;
          do{
            if(idNode->nodeType==NODE_TOKEN){
              if(!findSymbol(idNode->string)){
                addVariable(idNode->string,node->nodeType,node);
              }
            }
            idNode = idNode->rsibling;
          }while(idNode!=child);
          child = child->child;

        }while(child!=NULL);
        break;
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

