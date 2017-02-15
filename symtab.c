#include "node.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define help(s) {printf("\thelp: %s\n",s);}


struct SymTable *rootTable;

struct SymTable * newSymTable(struct SymTable * parent){
  struct SymTable *table = (struct SymTable*)malloc(sizeof(struct SymTable));
  table->size = 0;
  for(int i=0;i<100;i++){
    strcpy(table->entries[i].name, "");
  }
  table->parent = parent;
  return table;
}

struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s) {
    //struct SymTableEntry * entry;
    assert(s);
    
    if(SymbolTable->size !=0){
      for(int i=0; i<SymbolTable->size; i++) {
        if(strcmp(s, SymbolTable->entries[i].name) == 0) {
          return &SymbolTable->entries[i];
        }

      }
    }
    if(SymbolTable->parent !=0)
      return findSymbol(SymbolTable->parent, s);  
    else
      return 0;
}

struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link) {
    struct SymTable *SymbolTable = link->table;
    
    if(findSymbol(link->table, s) != 0) {
        printf("Error: duplicate declaration of variable %s\n", s);
        //exit(0);
    }

    int index = SymbolTable->size;
    (SymbolTable->size)++;

    strcpy(SymbolTable->entries[index].name, s);
    SymbolTable->entries[index].type = type;
    SymbolTable->entries[index].link = link;
    //printf("dump entry:%d, name:%s, type:%d, link:%x\n", index, 
    //    SymbolTable->entries[index].type,
    //    SymbolTable->entries[index].link);
    printf("table:0x%x, AddVariable:%s, valueType:%d\n", (unsigned int)SymbolTable, s, type);
    return &SymbolTable->entries[index];
}

void setTable(struct nodeType *node){
  switch(node->nodeType){
    case NODE_NESL:
      break;
    case NODE_APPLYBODY1:
    case NODE_APPLYBODY2:
    case NODE_APPLYBODY3:
    case NODE_APPLYBODY4:
    case NODE_FUNC:
    case NODE_LET:{
      //printf("nodetype:%d create scope oh yeah~\n",node->nodeType);
      struct SymTable * newScope = newSymTable(node->table);
      node->table = newScope;
      node->table->parent = node->parent->table;
      break;
    }
    default:
      assert(node->parent);
      node->table = node->parent->table;
      break;
  }

  struct nodeType * child = node->child;
  if(child!=0){
    do{
      setTable(child);
      assert(child->table);
      child = child->rsibling;
    }while(child!=node->child);
  }
  
}
struct nodeType* nthChild(int n, struct nodeType *node) {
    struct nodeType *child = node->child;
    for(int i=1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}

void tupleTransform(struct nodeType *node){
  struct nodeType * foo  = node->child->lsibling;
  if(foo->nodeType == NODE_TUPLE){
    struct nodeType *c1 = nthChild(1,foo);
    struct nodeType *c2 = nthChild(2,foo);
    c1->parent = node;
    c2->parent = node;
    c1->rsibling = c2;
    c2->lsibling = c1;
    c2->rsibling = node->child;
    node->child->lsibling = c2;
    c1->lsibling = foo->lsibling;
    foo->lsibling->rsibling = c1;
    tupleTransform(node);
  }
}

void removePair(struct nodeType *node){
  
  // doing the tupleTransform
  struct nodeType *child = node->child;
  struct nodeType *RHS = node->rsibling;
  struct nodeType *LHS = node->lsibling;
  int counts=0;
  int hasTuple=0;
  int i =0;
  switch(node->nodeType){
    case NODE_SEQ:
      if(child!=0){
        do{
          if(child->nodeType == NODE_TUPLE)
            hasTuple =1;
          counts++;
          child = child->rsibling;
        }while(child!=node->child);
      } 
      if(hasTuple){
          tupleTransform(node);
      }
      printf("SEQ:count:%d\n",counts);
      
      break;
    default:
      if(child!=0){
        do{
          removePair(child);
          child = child->rsibling;
        }while(child != node->child);
      }
      break;
  }
}


void typeBinding(struct nodeType *node1, struct nodeType *node2){
  switch(node2->nodeType){
    case NODE_TYPE_PAIR:
      node2= node2->child;
      typeBinding(node1, node2);
      return;
  }
  switch(node1->nodeType){
    case NODE_PATTERN:
    case NODE_PAIR:
      node1= node1->child;
      typeBinding(node1, node2);
      return;
    case NODE_TUPLE:{
      assert(node2->nodeType == NODE_TUPLE);
      struct nodeType * child1,*child2;
      child1 = node1->child;
      child2 = node2->child;
      if(node1->child!=0 && node2->child!=0){
        do{
          typeBinding(child1, child2);
          child1 = child1->rsibling;
          child2 = child2->rsibling;
        }while((child1!=node1->child) && (child2!=node2->child));
      }
      break;
    }
    case NODE_TOKEN:{
      switch(node2->nodeType){
      case NODE_SEQ_REF:
        
        break;
      case NODE_TUPLE:
        node1->valueType = TypeTuple;
        node1->typeNode = node2;
        addVariable(node1->string, TypeTuple, node1); 
        break;
      case NODE_TYPE_SEQ:
        switch(node2->child->nodeType){
          case NODE_TOKEN: // extract simple SEQ here.
            switch (node2->child->tokenType){
              case TOKE_INT:
                node1->valueType = TypeSEQ_I;
                node1->typeNode = node2;
                break;
              case TOKE_FLOAT:
                node1->valueType = TypeSEQ_F;
                node1->typeNode = node2;
                break;
              case TOKE_CHAR:
                node1->valueType = TypeSEQ_C;
                node1->typeNode = node2;
                break;
              case TOKE_BOOL:
                node1->valueType = TypeSEQ_B;
                node1->typeNode = node2;
                break;
            }
        }
        //node1->valueType = TypeSEQ;
        //node1->typeNode = node2;  
        addVariable(node1->string, node1->valueType, node1); 
        break;
      case NODE_APPLYBODY2:{
        addVariable(node1->string, node2->valueType, node1);
        node1->valueType = node2->valueType;
        node1->typeNode = node2;
        break;}
      case NODE_APPLYBODY3:{
        addVariable(node1->string, node2->valueType, node1);
        node1->valueType = node2->valueType;
        node1->typeNode = node2;
        break;}
      case NODE_TOKEN:
        switch(node2->tokenType){
        case TOKE_INT:
          node1->valueType = TypeInt;
          addVariable(node1->string, TypeInt, node1);
          break;
        case TOKE_FLOAT:
          node1->valueType = TypeFloat;
          addVariable(node1->string, TypeFloat, node1);
          break;
        case TOKE_BOOL:
          node1->valueType = TypeBool;
          addVariable(node1->string, TypeBool, node1);
          break;
        case TOKE_CHAR:
          node1->valueType = TypeChar;
          addVariable(node1->string, TypeChar, node1);
          break;
        default:
          addVariable(node1->string, node2->valueType, node1);
          //assert(0);
          break;
        }
        break;
      }
    }
  }
}


void typeAnalysis( struct nodeType *node){
  switch(node->nodeType){
    case NODE_NESL:
    case NODE_LIST:
    case NODE_RBINDS:{
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          typeAnalysis(child);
          child = child->rsibling; 
        }while(child!=node->child);
      }
      if(node->nodeType == NODE_RBINDS){
        node->valueType = node->child->valueType;
      }
      break;
    }

    case NODE_TUPLE:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      node->valueType = TypeTuple;
      node->typeNode = node;
      break;
    }
      
    case NODE_FUNC:{
      
      struct nodeType *inputParam = node->child;
      struct nodeType *typeDef = node->child->rsibling;
      struct nodeType *funcExp = typeDef->rsibling;
      
      // functions must have the typeDefinition of Function.
      assert(typeDef->nodeType  == NODE_OP);
      
      if(typeDef->op == OP_RARROW){
        // Bind the inputParameter with TypeDeclaration
        typeBinding(inputParam, typeDef->child);

        // Analyse the returnType of the function, RHS of op_rarrow.
        typeAnalysis(typeDef->child->rsibling);
        node->table = node->parent->table;
        addVariable(node->string, typeDef->child->rsibling->valueType, node);  
        
        // Assign the returnType to the functionNode
        typeAnalysis(node->child->rsibling->rsibling);
        node->valueType = node->child->rsibling->rsibling->valueType;
      
        switch(node->child->rsibling->rsibling->valueType){
          case TypeInt:
          case TypeBool:
          case TypeFloat:
          case TypeChar:
            break;
          case TypeSEQ_I:
          case TypeSEQ_F:
          case TypeSEQ_B:
          case TypeSEQ_C:
            break;
        }
        // Add the function node into SymbolTable
      }
      else {
        printf("error in node_func of semanticPass\n");
        return;
      } 
     break;
    }
   
    case NODE_TYPE_SEQ:{
      typeAnalysis(node->child);
      switch(node->child->valueType){
        case TypeInt:
          node->valueType = TypeSEQ_I;
          break;
        case TypeFloat :
          node->valueType = TypeSEQ_F;
          break;
        case TypeBool:
          node->valueType = TypeSEQ_B;
          break;
        case TypeChar:
          node->valueType = TypeSEQ_C;
          break;
        default:
          assert(0);
          // TODO 
          break;
        }
      break;}
    case NODE_IFELSE:{
      struct nodeType* ifstmt = node->child;
      struct nodeType* thstmt = node->child->rsibling;
      struct nodeType* elstmt = node->child->rsibling->rsibling;
      typeAnalysis(ifstmt);
      typeAnalysis(thstmt);
      typeAnalysis(elstmt);
      assert(ifstmt->valueType == TypeBool); 
      assert(thstmt->valueType == elstmt->valueType);
      node->valueType = elstmt->valueType;
      
      break;
    }
    case NODE_IFSTMT:
    case NODE_THENSTMT:
    case NODE_ELSESTMT:
    case NODE_TYPE_PAIR:
    case NODE_PAIR:{
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      if(node->valueType >= TypeSEQ){
        node->typeNode = node->child->typeNode;
      }
      break;
    } 
    
    case NODE_EXP:{
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      break;
    }
    case NODE_LET:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      node->valueType = node->child->rsibling->valueType;
      break;
    }
    case NODE_BIND:{
      struct nodeType * child = node->child;
      assert(node->childcount);
      for(int i =0; i<node->childcount;i++){
        typeAnalysis(child);
        child=child->rsibling;
      }
      
      break;
    }
    case NODE_SEQ_REF:{
      struct SymTableEntry * child = findSymbol(node->child->table, node->child->string);
      
      assert(child->type <= TypeSEQ);
      assert(child->type >= TypeSEQ_I);
      assert(child);
      switch(child->type){
        case TypeSEQ:
          //node->child
          switch(child->link->typeNode->nodeType){
            case NODE_APPLYBODY1:
              break;
            case NODE_APPLYBODY2:
              node->valueType = 
              child->link->typeNode->child->valueType;
              break;
            case NODE_APPLYBODY3:
              node->valueType = 
              child->link->typeNode->child->valueType;
              break;
            case NODE_APPLYBODY4:
              break;
            default:
              assert(0); //not implement;
          }
          //node->valueType = node->typeNode->valueType;
          //assert(0); // not implement yet; 
          break;
        case TypeSEQ_I:
          node->valueType = TypeInt;
          break;
        case TypeSEQ_C:
          node->valueType = TypeChar;
          break;
        case TypeSEQ_B:
          node->valueType = TypeBool;
          break;
        case TypeSEQ_F:
          node->valueType = TypeFloat;
          break;
        default:
          assert(0);// impossible;
          break;
      }
      // analyse the index part.
      struct nodeType *RHS = node->child->rsibling;
      typeAnalysis(RHS);
      assert(RHS->valueType==TypeInt);
      break;
    }

    case NODE_PATTERN:{
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      break;}

    case NODE_OP:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      typeAnalysis(LHS);
      if(RHS!=LHS)
        typeAnalysis(RHS);
      
      switch(node->op){
        case OP_BIND:
          assert(RHS->valueType);
          assert(LHS->nodeType == NODE_PATTERN);
          // might have pattern->pair->tuple-id&id,
          // can't directly addVariable.
          if(LHS->valueType==TypeTuple){
            
            // get RHS's typeDef
            struct nodeType* RHStypeNode = RHS->typeNode;
            struct nodeType* RHSchild = RHStypeNode->child;
            assert(RHStypeNode->valueType == TypeTuple);
            assert(RHStypeNode->child);
            assert(RHStypeNode->child->rsibling);

            //assert(LHS->typeNode);
            struct nodeType* tupleNode = LHS->child;
            while(tupleNode -> nodeType != NODE_TUPLE){
              if(tupleNode->child!=0)
                tupleNode = tupleNode->child;
            }
            struct nodeType* tuplechild = tupleNode->child;
            assert(tuplechild);
            
            // similar to TypeBinding
            if(tuplechild){
              do{
                assert(tuplechild->string);
                assert(RHSchild->valueType);
                addVariable(tuplechild->string, RHSchild->valueType, tuplechild);
                tuplechild = tuplechild->rsibling;
                RHSchild = RHSchild->rsibling;
              }while(tuplechild!=tupleNode->child);
            }
            

            //assert(RHS->valueType == TypeTuple);

          }else{
            addVariable(LHS->child->string, RHS->valueType, LHS);
          }
          //typeBinding(LHS, RHS);
          LHS->typeNode = RHS;
          typeAnalysis(LHS);
          node->valueType = RHS->valueType;

          break;
        case OP_ADD:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_SUB:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_MUL:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_DIV:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          break;
        case OP_RARROW:
          assert(LHS->valueType == RHS->valueType);
          break;
        case OP_AT:
          break;
        case OP_UMINUS:
          //assert(
          break;
        case OP_SHARP:{
          if(LHS->valueType >=TypeSEQ_I && LHS->valueType <= TypeSEQ ){
            node->valueType = TypeInt;
          }else{ 
            assert(0);
          }
          break;
        }
        case OP_GT:
        case OP_EQ:
        case OP_GE:
        case OP_LE:
        case OP_NE:
        case OP_LT:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = TypeBool;
          break;
        case OP_NOT:
          node->valueType = LHS->valueType;
          break;
        case OP_OR:
          break;
        case OP_COMMA:
          break;
        case OP_AND:
          break;
        case OP_NOR:
          break;
        case OP_NAND:
          break;
        case OP_XOR:
          break;
        case OP_LARROW:
          break;
        case OP_UPT:
          break;
        case OP_PP:
          assert(LHS->valueType == RHS->valueType);
          //assert(LHS->valueType == TypeSEQ);
          node->valueType = LHS->valueType;
          //FIXME not only TypeSEQ.
          break;
      }// end of node->op
      break;
    } // end of NODE_OP
     
    case NODE_FUNC_CALL:{
      // first, check the input parameter.
      typeAnalysis(node->child->rsibling);
      
      // second, check the function return type.
      assert(node->child->nodeType == NODE_TOKEN);
      assert(node->child->string);

      if(strcmp(node->child->string, "dist")==0){
          node->valueType = TypeSEQ_I;
          return;
          }
      else if(strcmp(node->child->string, "time")==0){
        //assert(0);
        node->valueType = TypeTuple;
        struct nodeType *refNode = newNode(NODE_TUPLE);
        struct nodeType *Lchild = newNode(NODE_TOKEN);
        struct nodeType *Rchild = newNode(NODE_TOKEN);
        addChild(refNode, Lchild);
        addChild(refNode, Rchild);
        Rchild->tokenType = TOKE_FLOAT;
        Rchild->valueType = TypeFloat;
        Rchild->table = node->table;
        refNode->table = node->table;
        refNode->valueType = TypeTuple;
        assert(node->child->rsibling->valueType);
        Lchild->table = node->table;
        Lchild->valueType = node->child->rsibling->valueType;
        node->typeNode = refNode;
        return;
      }else if(strcmp(node->child->string, "rand")==0){
        node->valueType = TypeInt;
        return;
      }
          // TODO other built-in functions
      typeAnalysis(node->child);
      
      // 1. search the node->child->string in built-in list
      //    if found then use the signature to check and 
      //    assign the type informations.
      // 2. if not found, then search the symbol table
      //    it should be a user define function.

      node->valueType = node->child->valueType;
      break;
    }
    case NODE_APPLYBODY1:{
      typeAnalysis(node->child);
      node->valueType = TypeSEQ;
      break;
    }
    case NODE_APPLYBODY2:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      switch(node->child->valueType){
        case TypeInt:
          node->valueType = TypeSEQ_I;
          break;
        case TypeFloat:
          node->valueType = TypeSEQ_F;
          break;
        case TypeBool:
          node->valueType = TypeSEQ_B;
          break;
        case TypeChar:
          node->valueType = TypeSEQ_C;
          break;
        default:
          node->valueType = TypeSEQ;
          break;
      }
      //node->valueType = node->child->valueType;
      node->typeNode = node->child->rsibling->typeNode;
      break;
    }
    case NODE_APPLYBODY3:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      assert(node->child->rsibling->valueType == TypeBool);
      assert(node->child->valueType);
      node->valueType = node->child->valueType;
      node->typeNode = node->child->child;
      break;
    }
    case NODE_APPLYBODY4:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      typeAnalysis(node->child->lsibling);
      node->valueType = TypeSEQ;
      assert(node->child->lsibling->valueType == TypeBool);
      break;
    }
    case NODE_IN:{
      // TODO e in a, e should be renamed or replaced from a var list.
      //typeAnalysis(node->child);
      
      typeAnalysis(node->child->rsibling);
      node->valueType = node->child->rsibling->valueType;
      
      //FIXME whether to use the typeBinding or processing here?
      switch(node->child->rsibling->valueType){
        case TypeSEQ_I:
          node->child->valueType = TypeInt;
          break;
        case TypeSEQ_F:
          node->child->valueType = TypeFloat;
          break;
        case TypeSEQ_B:
          node->child->valueType = TypeBool;
          break;
        case TypeSEQ_C:
          node->child->valueType = TypeChar;
          break;
        case TypeSEQ:
          assert(node->child->rsibling->child->valueType);
          node->child->valueType = node->child->rsibling->child->valueType;
          node->typeNode = node->child->rsibling;
          break;
        
        default:
          // not implement
          assert(0);
        break;
      }
         if(!findSymbol(node->table, node->child->string))
           addVariable(node->child->string, 
                        node->child->valueType,
                        node->child);
      break;}

    case NODE_TOKEN: {
        switch(node->tokenType){
          case TOKE_ID:{
            struct SymTableEntry* entry = findSymbol(node->table, node->string);
            if(entry)
              node->valueType = entry->type; 
            //else //FIXME
              
              //addVariable(node->string, TypeInt, node);

            break;
            }
          case TOKE_INT:
            node->valueType = TypeInt;
            break;
          case TOKE_FLOAT:
            node->valueType = TypeFloat;
            break;
          case TOKE_BOOL:
            node->valueType = TypeBool;
            break;
          case TOKE_CHAR:
            node->valueType = TypeChar;
            break;
          case TOKE_STRING:
            node->valueType = TypeSEQ_C;
            break;
          default:
            break;
        }// end of switch tokenType
      break;
    }// end of NODE_TOKEN
    
    case NODE_INT:{
      node->valueType = TypeInt;
      break;
    }
    case NODE_BOOL:{
      node->valueType = TypeBool;
      break;
    }
    case NODE_FLOAT:{
      node->valueType = TypeFloat;
      break;
    }
    case NODE_CHAR:{
      node->valueType = TypeChar;
      break;
    }
    case NODE_FILTER:{
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      break;
    }
    case NODE_NEW_SEQ:{
      struct nodeType *child = node->child;
      int count=0;
      assert(child);
      if(child){
        do{
          count++;
          typeAnalysis(child);
          child = child->rsibling;
        }while(child!=node->child);
      }
      for(int i=0;i<count-1;i++){
        assert(child->valueType);
        assert(child->valueType == child->rsibling->valueType);
        child = child->rsibling;
      }
      
      switch(child->valueType){
        case TypeInt: 
          node->valueType = TypeSEQ_I;
          break;
        case TypeFloat:
          node->valueType = TypeSEQ_F;
          break;
        case TypeBool:
          node->valueType = TypeSEQ_B;
          break;
        case TypeChar:
          node->valueType = TypeSEQ_C;
          break;
        default:
          node->valueType = TypeSEQ;
          node->typeNode = node->child;
          break;
      }
      
      break;
    }
    case NODE_SEQ:{
        struct nodeType * child= node->child;
        assert(child);
        typeAnalysis(child);
        switch(child->valueType){
          case TypeInt:
            node->valueType = TypeSEQ_I;
            break;
          case TypeFloat:
            node->valueType = TypeSEQ_F;
            break;
          case TypeBool:
            node->valueType = TypeSEQ_B;
            break;
          case TypeChar:
            node->valueType = TypeSEQ_C;
            break;
          case TypeSEQ_I:
          case TypeSEQ_F:
          case TypeSEQ_B:
          case TypeSEQ_C:
          default:
            node->valueType = TypeSEQ;
            break;
        }

      break;
    }
    
  }// end of switch nodeType

}//end of typeAnalysis

//void setTable(struct nodeType *node, struct SymTable *table){


void semanticPass( struct nodeType *node){
  // create and set a table in rootNode. 
  // then start the Analysis
  // which consist of table insertion, 
  // typeBinding and typeChechking.
  rootTable = newSymTable(NULL);
  node->table = rootTable;
  setTable(node);
  typeAnalysis(node);
}

void printNESL(struct nodeType *node, FILE* yyout){
  switch(node->nodeType){
    case NODE_NESL:
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
      //fprintf(yyout,"(");
      printNESL(child, yyout);
      //fprintf(yyout,")");
      fprintf(yyout,":\n");
      printNESL(child->rsibling, yyout);
      fprintf(yyout,"=\n");
      printNESL(child->rsibling->rsibling, yyout);
      fprintf(yyout,";\n");
      break;
    }
    case NODE_TUPLE:{
     struct nodeType *child = node->child;
      //fprintf(yyout,"(");
      printNESL(child, yyout);
      child = child->rsibling;
      while(child!=node->child){
        fprintf(yyout,",");
        printNESL(child, yyout);
        child = child->rsibling;
      }
      //fprintf(yyout,")");
      break;
    }
    case NODE_PAIR:
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
          fprintf(yyout,"(");
          printNESL(node->child, yyout);
          fprintf(yyout,")");
          fprintf(yyout,"->");
          fprintf(yyout,"(");
          printNESL(node->child->rsibling, yyout);
          fprintf(yyout,")");
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
      int counts=0;
      if(child!=0){
        do{
          counts++;
          printNESL(child, yyout);
          fprintf(yyout,";\n");
          child = child->rsibling;
        }while(child!=node->child);
      node->childcount = counts;
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
    case NODE_NEW_SEQ:
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
      fprintf(yyout," then ");
      printNESL(node->child, yyout);
      fprintf(yyout,"\n");
      break;
    }
    case NODE_ELSESTMT:{
      fprintf(yyout," else \n");
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
//void semanticCheck(struct nodeType *node) {
// //   printf("nodetype:%d\n", node->nodeType);
//    switch(node->nodeType){
//      case NODE_FUNC:
//      {
//        
//        if(!findSymbol(node->table, node->string)){
//          int vtype = node->child->rsibling->child->child->rsibling->valueType;
//          addVariable(node->string, vtype, node);
//        }
//        break;
//      }
//
//      case NODE_PATTERN:
//      {
//        
//        struct nodeType *child = node->child;
//        struct nodeType *idNode = child;
//        do{
//          idNode=child;
//          do{
//            if(idNode->nodeType==NODE_TOKEN){
//              if(!findSymbol(node->table, idNode->string)){
//                addVariable(idNode->string,node->nodeType,node);
//              }
//            }
//            idNode = idNode->rsibling;
//          }while(idNode!=child);
//          child = child->child;
//
//        }while(child!=NULL);
//        break;
//      }
//    }
//    
//    /* Default action for other nodes not listed in the switch-case */
//    struct nodeType *child = node->child;
//    if(child != 0) {
//        do {
//            semanticCheck(child);
//            child = child->rsibling;
//        } while(child != node->child);
//    }
//}

