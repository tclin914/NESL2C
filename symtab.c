#include "node.h"
#include "symtab.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    SymbolTable->entries[index].isParam = link->isParam;
    //printf("dump entry:%d, name:%s, type:%d, link:%x\n", index, 
    //    SymbolTable->entries[index].type,
    //    SymbolTable->entries[index].link);
    printf("table:0x%p, AddVariable:%s, valueType:%d\n", SymbolTable, s, type);
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
    case NODE_PAIR:
    case NODE_TYPE_PAIR:
      node2= node2->child;
      typeBinding(node1, node2);
      return;
  }
  switch(node1->nodeType){
    case NODE_PATTERN:
    case NODE_PAIR:
      node1= node1->child;
      node1->isParam = node1->parent->isParam;
      typeBinding(node1, node2);
      return;
    case RB_TUPLE:{
      struct nodeType *child1 = node1->child;
      struct nodeType *child2 = node2->child;
      typeBinding(child1,child2);
    break;
    }
    case NODE_TUPLE:{
      assert(node2->nodeType == NODE_TUPLE);
      struct nodeType * child1,*child2;
      child1 = node1->child;
      child1->isParam = node1->isParam;
      child2 = node2->child;
      if(child1 && child2){
        do{
          typeBinding(child1, child2);
          child1 = child1->rsibling;
          child1->isParam = node1->isParam;
          child2 = child2->rsibling;
        }while((child1!=node1->child) && (child2!=node2->child));
      }
      node1->valueType = node2->valueType;
      break;
    }
    case NODE_TOKEN:{
      switch(node2->nodeType){
      case NODE_SEQ_REF:
        
        break;
      case NODE_TUPLE:
        typeAnalysis(node2);
        node1->valueType = node2->valueType;
        node1->typeNode = node2;
        addVariable(node1->string, node2->valueType, node1); 
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
              default:
                assert(0); // not implement;
                break;
            }
            addVariable(node1->string, node1->valueType, node1);
            break;
          case NODE_TYPE_PAIR:
            if(node2->child->child->nodeType == NODE_TUPLE){
               node1->typeNode = node2->child->child;
               node1->valueType = TypeSEQ;
               addVariable(node1->string, node1->valueType, node1);
            }else {
              assert(0); // not implement;
            }
          break;
        }
        //node1->valueType = TypeSEQ;
        //node1->typeNode = node2;  
        //addVariable(node1->string, node1->valueType, node1); 
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

      int count = 0;
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          count++;
          typeAnalysis(child);
          child = child->rsibling; 
        }while(child!=node->child);
      }
      node->counts = count;
      if(node->nodeType == NODE_RBINDS){
        node->valueType = node->child->valueType;
      }
      break;
    }
    case ELEM_TUPLE:
    case NODE_TUPLE:{
      typeAnalysis(node->child);
      typeAnalysis(node->child->rsibling);
      node->valueType = TypeTuple;
      node->typeNode = node;
      if(node->child->valueType == node->child->rsibling->valueType){
        switch(node->child->valueType){
          case TypeInt: 
            node->valueType = TypeTuple_I;
          break;
          case TypeFloat: 
            node->valueType = TypeTuple_F;
          break;
          case TypeBool: 
            node->valueType = TypeTuple_B;
          break;
          case TypeChar: 
            node->valueType = TypeTuple_C;
          break;
        }
      }
      else{
        switch (node->child->valueType){
        case TypeInt:
          switch(node->child->rsibling->valueType){
          case TypeFloat:
            node->valueType = TypeTuple_IF;
          break;
          }
        break;
        case TypeFloat:
          switch(node->child->rsibling->valueType){
          case TypeInt:
            node->valueType = TypeTuple_FI;
          break;
          }
        break;
        case TypeBool:
        case TypeChar:
          assert(0);
          break;
        break;
        case TypeSEQ_I:
        case TypeSEQ_F:
        case TypeSEQ_B:
        case TypeSEQ_C:
        case TypeSEQ:
          switch(node->child->rsibling->valueType){
          case TypeFloat:
            node->valueType = TypeTuple_SF;
          break;
          }
        break;
        }
      
      } 
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
        inputParam->isParam=1;
        typeBinding(inputParam, typeDef->child);
        typeAnalysis(typeDef->child);

        // Analyse the returnType of the function, RHS of op_rarrow.
        typeAnalysis(typeDef->child->rsibling);
        node->table = node->parent->table;
        node->isParam = 1;
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
          node->valueType = TypeSEQ;
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
      
      // this assumes they have been typeBinding.
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
      struct nodeType *child = node->child;
      int counts=0;
      if(child!=0){
        do{
          counts++;
          typeAnalysis(child);
          child = child->rsibling;
        }while(child!=node->child);
      node->childcount = counts;
      }
      break;
    }
    case NODE_SEQ_REF:{
      struct SymTableEntry * entry = findSymbol(node->child->table, node->child->string);
      
      assert(entry);
      assert(entry->type <= TypeSEQ);
      assert(entry->type >= TypeSEQ_I);
      switch(entry->type){
        case TypeSEQ:
          //node->child
          switch(entry->link->typeNode->nodeType){
            case NODE_APPLYBODY1:
              break;
            case NODE_APPLYBODY2:
              node->valueType = 
              entry->link->typeNode->child->valueType;
              break;
            case NODE_APPLYBODY3:
              node->valueType = 
              entry->link->typeNode->child->valueType;
              break;
            case NODE_APPLYBODY4:
              break;
            case NODE_TUPLE:
              node->valueType=
              entry->link->typeNode->valueType;
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
      case OP_BIND:{
        assert(RHS->valueType);
        LHS->valueType = RHS->valueType;
        if(node->parent->nodeType == NODE_NESL)
          LHS->isParam = 1;
        if(LHS->nodeType == NODE_PATTERN){
          // might have pattern->pair->tuple-id&id,
          // can't directly addVariable.
          if(LHS->valueType>=TypeTuple_I){
            
            switch(RHS->nodeType){
              case NODE_TOKEN:
              {// ex: (xo,yo)= o ;
              struct SymTableEntry *entry = findSymbol(node->table,RHS->string);
              assert(entry);
              typeBinding(LHS,entry->link->typeNode);
              break;}
              case NODE_FUNC_CALL:{
                assert(RHS->typeNode);
                typeBinding(LHS,RHS->typeNode);
              break;}
              case NODE_SEQ_REF:{
              //minx = points[min_index(x)];
              struct SymTableEntry *entry = findSymbol(node->table,RHS->child->string);
              assert(entry);
              LHS->typeNode = entry->link->typeNode;
              addVariable(LHS->child->string, LHS->valueType, LHS);
              //assert(0);
              break;}
            }
          }else if(LHS->valueType == TypeTuple_SF){
            assert(RHS->typeNode);
            typeBinding(LHS,RHS->typeNode);
            break;
          }else{
            addVariable(LHS->child->string, RHS->valueType, LHS);
          }
          //typeBinding(LHS, RHS);

          //FIXME   pattern
          //           |
          //        TOKEN_ID
          LHS->child->typeNode = RHS;
          LHS->typeNode = RHS;
          typeAnalysis(LHS);
          node->valueType = RHS->valueType;
        }// end of if LHS == PATTERN
        else if(LHS->valueType >=TypeTuple_I && LHS->nodeType == NODE_PAIR){
          if(RHS->nodeType == NODE_PAIR){
            typeBinding(LHS,RHS);
          }
        }
        else if(LHS->nodeType==NODE_TOKEN){
          LHS->valueType = RHS->valueType;
          addVariable(LHS->string, RHS->valueType, LHS);
        }else{
          assert(0); // not implement
        }
        break;
      }
        case OP_ADD:
          assert(LHS->valueType == RHS->valueType);
          node->valueType = RHS->valueType;
          assert(node->valueType<=TypeFloat);
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
          assert(RHS->valueType == TypeInt);
          node->valueType = LHS->valueType;
          break;
        case OP_PP:
          assert(LHS->valueType == RHS->valueType);
          //assert(LHS->valueType == TypeSEQ);
          node->valueType = LHS->valueType;
          //FIXME not only TypeSEQ.
        break;
        default:
        assert(0); // not implement;
      }// end of node->op
      break;
    } // end of NODE_OP
     
    case NODE_FUNC_CALL:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = LHS->rsibling;
      // first, check the input parameter.
      typeAnalysis(RHS);
      
      // second, check the function return type.
      assert(node->child->nodeType == NODE_TOKEN);
      assert(node->child->string);
      
      if(strcmp(node->child->string, "isContiguousList")==0){
          node->valueType = TypeInt;
          return;
          }
      else if(strcmp(node->child->string, "dist")==0){
          switch(RHS->child->child->valueType){
          case TypeInt:
          node->valueType = TypeSEQ_I;
          break;
          case TypeFloat:
            node->valueType = TypeSEQ_F;
          break;
          default:
          assert(0);
          break;
          }
          return;
          }
      else if(strcmp(node->child->string, "time")==0){
        // sequential version
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
        Lchild->tokenType = -1;
        Lchild->valueType = node->child->rsibling->valueType;
        node->typeNode = refNode;
        typeAnalysis(refNode);
        node->valueType = refNode->valueType;
        return;
      }else if(strcmp(node->child->string, "rand")==0){
        node->valueType = RHS->valueType;
        return;
      }else if(strcmp(node->child->string, "plusp") ==0){
        node->valueType = TypeBool;
        return;
      }else if(strcmp(node->child->string, "flatten") == 0){
        assert(RHS->valueType==TypeSEQ);
        node->valueType = RHS->typeNode->valueType;
        node->typeNode = RHS->typeNode;
        return;
      }else if(strcmp(LHS->string, "max_index") == 0){
        node->valueType = TypeInt;
        return;
      }else if(strcmp(LHS->string, "min_index") == 0){
        node->valueType = TypeInt;
        return;
      }else if(strcmp(LHS->string, "float") == 0){
        node->valueType = TypeFloat;
        return;
      }else if(strcmp(LHS->string, "round") == 0){
        node->valueType = TypeInt;
        assert(RHS->valueType == TypeFloat);
        return;
      }else if(strcmp(LHS->string, "print_string") == 0){
        node->valueType = TypeBool;
        assert(RHS->valueType == TypeSEQ_C);
        return;
      }else if(strcmp(LHS->string, "sum") == 0){
        switch(RHS->valueType){
        case TypeSEQ_I:
          node->valueType = TypeInt;
        break;
        case TypeSEQ_F:
          node->valueType = TypeFloat;
        break;
        case TypeSEQ:
          node->valueType = RHS->typeNode->valueType;
          break;
        default:
          assert(0);// error
        }  
        return;
      }else{
          // TODO other built-in functions
      typeAnalysis(node->child);
      struct SymTableEntry *entry = findSymbol(node->table, node->child->string); 
      assert(entry);
      assert(entry->type);
      node->valueType = entry->type;
      
      }
      // 1. search the node->child->string in built-in list
      //    if found then use the signature to check and 
      //    assign the type informations.
      // 2. if not found, then search the symbol table
      //    it should be a user define function.
      
      //node->valueType = node->child->valueType;
      break;
    }
    case NODE_APPLYBODY1:{
      typeAnalysis(node->child);
      node->valueType = TypeSEQ;
      break;
    }
    case NODE_APPLYBODY2:{
      // FIXME analyze child->rsibling first?
      typeAnalysis(node->child->rsibling);
      typeAnalysis(node->child);
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
      node->typeNode = node->child;
      break;
    }
    case NODE_APPLYBODY3:{
      // FIXME shouldn't have switch case here ?
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
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      // TODO e in a, e should be renamed or replaced from a var list.
      //typeAnalysis(node->child);
      
      typeAnalysis(RHS);
      node->valueType = RHS->valueType;
      
      //FIXME whether to use the typeBinding or processing here?
      switch(RHS->valueType){
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
          if(RHS->nodeType == NODE_TOKEN){
            struct SymTableEntry *entry = findSymbol(node->table,RHS->string);
            assert(entry); 
            struct nodeType* refNode = entry->link->typeNode;
            LHS->valueType = refNode->valueType;
            LHS->typeNode = refNode;
            node->typeNode = refNode;
          }else{
          assert(node->child->rsibling->child->valueType);
          node->child->valueType = node->child->rsibling->child->valueType;
          node->typeNode = node->child->rsibling;
          }
          break;
        default:
          // not implement
          assert(0);
        break;
      }

      // ValueType of LHS need to be derived downward.
      switch(LHS->nodeType){
      case NODE_TOKEN:
        if(!findSymbol(node->table, node->child->string))
          addVariable(node->child->string, 
                      node->child->valueType,
                      node->child);
      break;
      case NODE_TUPLE:{
        struct nodeType * refNode = LHS->typeNode;
        int count=0;
        node->child->nodeType = RB_TUPLE;
        while(refNode->valueType<=TypeTuple_I){
          refNode = refNode->child;
          count ++;
          if(count==10) assert(0);//refNode error.
        }
        assert(refNode);
        LHS->typeNode = refNode;
        typeAnalysis(LHS);
        
      break;}
      case NODE_PAIR:{
        // remove pair;
        struct nodeType * child = node->child;
        struct nodeType * rhs = node->child->rsibling;
        struct nodeType * lhs = node->child->lsibling;
        struct nodeType * pchild = node->child->child;
        
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        node->child = child;
        
        if(node->child->nodeType == NODE_TUPLE){
          struct nodeType* LHS = node->child;
          struct nodeType *RHS = LHS->rsibling;
          
          LHS->nodeType = RB_TUPLE;
          LHS->valueType = node->typeNode->valueType;
          LHS->typeNode = node->typeNode;
          assert(RHS->valueType ==9);
          switch(RHS->nodeType){
          case NODE_NEW_SEQ:
          assert(RHS->child->valueType>=10);
          LHS->typeNode = RHS->typeNode;
          typeAnalysis(LHS);
          break;
          case NODE_TOKEN:{
            struct SymTableEntry *entry = findSymbol(RHS->table, RHS->string);
            assert(entry); 
            node->typeNode = entry->link->typeNode;
            typeAnalysis(LHS);
          break;}
          default:
          assert(0);//not implement;
          break;
          }
        }else{
          if(!findSymbol(node->table, node->child->string))
            addVariable(node->child->string, 
                         node->child->valueType,
                         node->child);
          
        }
        
      break;
      }
      default:
        assert(0);//impossible
      break;
      }
      //FIXME consider the 
      
      //typeBinding(node->child,node->child->rsibling->typeNode);
      break;
    }// end of NODE_IN

    case RB_TUPLE:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      struct nodeType* lref = node->typeNode->child;
      struct nodeType* rref = lref->rsibling;

      if(LHS->nodeType == NODE_TUPLE){
        LHS->nodeType = RB_TUPLE;
        typeAnalysis(LHS);
      }else if(LHS->nodeType == NODE_TOKEN){
           if(!findSymbol(LHS->table, LHS->string)){
            addVariable(LHS->string, lref->valueType, LHS);
            LHS->valueType = lref->valueType;
           }
      }else{
        assert(0);
      }
      
      if(RHS->nodeType == NODE_TUPLE){
        RHS->nodeType = RB_TUPLE;
        typeAnalysis(RHS);
      }else if(RHS->nodeType == NODE_TOKEN){
           if(!findSymbol(RHS->table, RHS->string)){
            addVariable(RHS->string, rref->valueType, RHS);
            RHS->valueType = rref->valueType;
           }
      }else{
        assert(0);
      }

    break;
    }

    case NODE_TOKEN: {
        switch(node->tokenType){
          case TOKE_ID:{
            struct SymTableEntry* entry = findSymbol(node->table, node->string);
            if(entry)
              node->valueType = entry->type; 
            //else //FIXME
              if(node->valueType==TypeSEQ || node->valueType == TypeTuple){
                node->typeNode = entry->link->typeNode;
              }
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
      if(strcmp(node->string,"f")==0){
        node->iValue=0;
      }else if(strcmp(node->string,"F")==0){
        node->iValue=0;
      }else if(strcmp(node->string,"t")==0){
        node->iValue=1;
      }else if(strcmp(node->string,"T")==0){
        node->iValue=1;
      }

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
    case NODE_STRING:{
      node->valueType = TypeSEQ_C;
      break;
    }
    case NODE_FILTER:{
      int count = 0;
      struct nodeType *child = node->child;
      if(child){
        do{
          count++;
        }while(child!= node->child);
      }
      node->counts = count;
      typeAnalysis(node->child);
      node->valueType = node->child->valueType;
      break;
    }
    case NODE_NEW_SEQ:{
      struct nodeType *LHS = node->child;
      struct nodeType *RHS = node->child->rsibling;
      int count=0;
      assert(LHS);
      //paramcount = idx:  0, 1, 2
      //                  [1, 2, 3]
      //                   new_seq 
      //                    /  \
      //                   1  tuple
      //                       / \
      //                      2   3
      
      // transform pair-tuple into elem_tuple(remove pair)
      // this will help type inference.
      if(LHS->nodeType == NODE_PAIR && LHS->child->nodeType == NODE_TUPLE){
        // remove pair;
        struct nodeType * child = LHS; //child = node->child;
        struct nodeType * rhs = LHS->rsibling;
        struct nodeType * lhs = LHS->lsibling;
        struct nodeType * pchild = LHS->child;
        LHS->child->nodeType = ELEM_TUPLE; 
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        node->child = child;
        LHS=node->child;
      }


      // below consider no pair_node in tree and 
      typeAnalysis(LHS);
      LHS->paramcount = 0;
      if(RHS->nodeType == NODE_TUPLE){
        RHS->nodeType = NODE_SEQ_TUPLE;
        RHS->paramcount = 1;
        typeAnalysis(RHS);
        assert(LHS->valueType == RHS->valueType);
        node->counts = 1+RHS->counts;
      }
      else{
        if(RHS->nodeType == NODE_PAIR && RHS->child->nodeType == NODE_TUPLE){
          // remove pair;
          struct nodeType * child = RHS; //child = node->child;
          struct nodeType * rhs = RHS->rsibling;
          struct nodeType * lhs = RHS->lsibling;
          struct nodeType * pchild = RHS->child;
          RHS->child->nodeType = ELEM_TUPLE;
          while(child->nodeType == NODE_PAIR){
            pchild->parent = node;
            rhs->lsibling = pchild;
            lhs->rsibling = pchild;
            child = pchild;
            child->rsibling = rhs;
            child->lsibling = lhs;
            rhs = child->rsibling;
            lhs = child->lsibling;
            pchild = child->child;
          }
          RHS=LHS->rsibling;
        }
        typeAnalysis(RHS);
        RHS->paramcount =1;
      }// end of else.
      
      switch(LHS->valueType){
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

    //case ELEM_TUPLE:{
    //  struct nodeType* LHS = node->child;
    //  struct nodeType* RHS = node->child->rsibling;

    //  //LHS and RHS should be simple types
    //  // at most TypeSEQ_I (like sth in quicksort: [lesser, greater]);
    //  typeAnalysis(LHS);
    //  typeAnalysis(RHS);
    //  switch(

    //break;
    //}
    case NODE_SEQ_TUPLE:{
      struct nodeType* LHS = node->child;
      struct nodeType* RHS = node->child->rsibling;
      
      if(LHS->nodeType == NODE_PAIR && LHS->child->nodeType == NODE_TUPLE){
        // remove pair;
        struct nodeType * child = LHS; //child = node->child;
        struct nodeType * rhs = LHS->rsibling;
        struct nodeType * lhs = LHS->lsibling;
        struct nodeType * pchild = LHS->child;
        
        LHS->child->nodeType = ELEM_TUPLE;
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        node->child = child; 
        LHS = node->child;
      }
      if(RHS->nodeType == NODE_PAIR && RHS->child->nodeType == NODE_TUPLE){
        // remove pair;
        struct nodeType * child = RHS; //child = node->child;
        struct nodeType * rhs = RHS->rsibling;
        struct nodeType * lhs = RHS->lsibling;
        struct nodeType * pchild = RHS->child;
        RHS->child->nodeType = ELEM_TUPLE;
        while(child->nodeType == NODE_PAIR){
          pchild->parent = node;
          rhs->lsibling = pchild;
          lhs->rsibling = pchild;
          child = pchild;
          child->rsibling = rhs;
          child->lsibling = lhs;
          rhs = child->rsibling;
          lhs = child->lsibling;
          pchild = child->child;
        }
        RHS = LHS->rsibling;
      }
      
      typeAnalysis(LHS);
      node->valueType = LHS->valueType;
      LHS->paramcount = node->paramcount;
      if(RHS->nodeType == NODE_TUPLE){
        RHS->nodeType = NODE_SEQ_TUPLE;
        RHS->paramcount = node->paramcount + 1;
        typeAnalysis(RHS);
        assert(LHS->valueType == RHS->child->valueType);
        node->counts = RHS->counts+1;
      }else{
        typeAnalysis(RHS);
        assert(LHS->valueType == RHS->valueType);
        node->counts = 2;
        RHS->paramcount = node->paramcount +1;
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
            node->typeNode = child;
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
