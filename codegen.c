#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "symtab.h"
#include <assert.h>
#include "node.h"
#include "codegen.h"
#define help(s) {printf("\thelp: %s\n",s);}


void printNESL(struct nodeType *node, FILE* yyout){
  switch(node->nodeType){
    case NODE_NESL:
    case NODE_LIST:{
      int count = 0;
      struct nodeType *child = node->child;
      if(child!=0){
        do{
          count++;
          printNESL(child, yyout);
          fprintf(yyout, "\n");
          child = child->rsibling;
        }while( child!=node->child);
      }
      node->counts = count;
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
        case OP_GT:     
          printNESL(node->child, yyout); 
          fprintf(yyout,">"); 
          printNESL(node->child->rsibling, yyout);  
          break;
        case OP_EQ:     
          printNESL(node->child, yyout); 
          fprintf(yyout," == "); 
          printNESL(node->child->rsibling, yyout);  
          break;
        case OP_GE:     
          printNESL(node->child, yyout); 
          fprintf(yyout," >= "); 
          printNESL(node->child->rsibling, yyout);  
          break;
        case OP_LE:     
          printNESL(node->child, yyout); 
          fprintf(yyout," <= "); 
          printNESL(node->child->rsibling, yyout); 
          break;
        case OP_NE:     
          printNESL(node->child, yyout);
          fprintf(yyout," != ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_NOT:    
          printNESL(node->child, yyout);
          fprintf(yyout," NOT ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_OR:    
          printNESL(node->child, yyout);
          fprintf(yyout," OR ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_COMMA: 
          printNESL(node->child, yyout);
          fprintf(yyout," , ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_AND:   
          printNESL(node->child, yyout);
          fprintf(yyout," AND ");
          printNESL(node->child->rsibling, yyout);  
          break;
        case OP_NOR:   
          printNESL(node->child, yyout);
          fprintf(yyout," NOR ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_NAND:   
          printNESL(node->child, yyout); 
          fprintf(yyout," NAND ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_XOR:    
          printNESL(node->child, yyout); 
          fprintf(yyout," XOR ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_LARROW: 
          printNESL(node->child, yyout);
          fprintf(yyout," <- ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_UPT:    
          printNESL(node->child, yyout);
          fprintf(yyout," ^ ");
          printNESL(node->child->rsibling, yyout);
          break;
        case OP_PP:     
          printNESL(node->child, yyout); 
          fprintf(yyout," ++ ");
          printNESL(node->child->rsibling, yyout); 
          break;
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

}// End of printNESL

void dumpTable(FILE *fptr, struct nodeType* node){
  struct SymTable * table = node->table;
  for(int i = 0; i<table->size ; i++){
    if(table->entries[i].link->nodeType!=NODE_FUNC){
      switch(table->entries[i].type){
        case TypeInt :
          fprintf(fptr, "int %s;\n",table->entries[i].name);
          break;
        case TypeFloat :
          fprintf(fptr, "float %s;\n",table->entries[i].name);
          break;
        case TypeBool :
          fprintf(fptr, "bool %s;\n",table->entries[i].name);
          break;
        case TypeChar :
          fprintf(fptr, "char* %s = malloc(sizeof(char*100));\n",table->entries[i].name);
          break;
        case TypeSEQ_I:
          fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
          break;
        case TypeSEQ_F:
          fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
          break;
        case TypeSEQ_B:
          fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
          break;
        case TypeSEQ_C:
          fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
          break;
        case TypeTuple:
          fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
          break;
        case TypeSEQ:
          fprintf(fptr, "struct Sequence %s;\n", table->entries[i].name);
          break;
        default:
          fclose(fptr);
          assert(0);//not implement;
          break;
      }//end of switch(entry->type)
    }//end of if
    
  }//end of for.

}// end of dumpTable.

void printparam(FILE *fptr, struct nodeType* node){
 switch(node->nodeType){
  case NODE_PAIR:
    fprintf(fptr, "(");
    printparam(fptr, node->child);
    fprintf(fptr, ")");
    break;
  case NODE_TOKEN:{
    struct nodeType *refNode = node->typeNode;
    printparam(fptr, refNode);
    fprintf(fptr, " %s", node->string);
    break;
  }
  case NODE_TYPE_SEQ:{
    switch(node->valueType){
      case TypeSEQ_I:
        fprintf(fptr, "struct Sequence");
        break;
      default :
        //TODO
        break;
    }
    break;
  }
 } 

}


void APP3printFor(FILE *fptr, struct nodeType* node1, struct nodeType* node2){
  char seqname[100];
  strcpy(seqname, node1->child->child->rsibling->string);
  fprintf(fptr, "for(int i = 0, count =0; i<%s.len; i++){\n", seqname);
  
  // ugly declare the "e" by dump it.
  dumpTable(fptr, node2->child);
  
  // and initialize it by assigning an element value.
  fprintf(fptr, "  %s = %s.ptr[i];\n",node1->child->child->string,seqname);
  
  // print the FILTER 
  // may also use boolean guard here.
  fprintf(fptr, "  if(");

  assert(node2->nodeType == NODE_FILTER);
  codegen(fptr, node2->child);
  fprintf(fptr, "){\n");
  
  fprintf(fptr, "    %s.ptr[count] = %s.ptr[i];\n    count++;\n    %s.len++;\n}\n", 
          node1->parent->rsibling->child->string,
          seqname,
          node1->parent->rsibling->child->string
          );
  fprintf(fptr, "}\n");
  
}

void printNewSeq(FILE *fptr, struct nodeType* node){
  
  // this should be called from APP3printFor
  // aim to allocate the tmp array v in quicksort.

  assert(node->nodeType == NODE_IN);
  struct nodeType *elm = node->child;
  struct nodeType *newSEQ = node->child->rsibling;
  //fprintf(fptr, "for( int i =0;  i < %d; i++){\n", newSEQ->counts);
  struct nodeType *newelm = newSEQ->child;
  fprintf(fptr, "%s = malloc(sizeof(%s)*%d);\n",elm->string, 
                                        newelm->string, newSEQ->counts);
  for(int i =0;i<newSEQ->counts;i++){
    fprintf(fptr, "%s.ptr[i] = %s;\n",elm->string,newelm->string);
    newelm = newelm->rsibling;
  }

}

void APP2printFor(FILE *fptr, struct nodeType* node1, struct nodeType* node2){
  //char[100] seqname = node2->child->child->
  
  // below for qs.nesl
  struct nodeType *newseq = node2->child->child->rsibling;
  if(newseq!=0 && newseq->nodeType == NODE_NEW_SEQ){
    // node1 might be functioncall or node_op;
    // here consider node_op firstly.
    fprintf(fptr, "{\n");
    dumpTable(fptr, node1);
    printNewSeq(fptr, node2->child);  
    //fprintf(fptr, "for( int i =0; i<%d;i++){\n", newseq->counts);
    //fprintf(fptr, "  %s.ptr[i] = %s;\n"
    
    switch(node1->nodeType){
      case NODE_FUNC_CALL:{
        struct nodeType *funcName = node1->child;
        struct nodeType *funcPara = funcName->rsibling;// usually a node pair.
        fprintf(fptr, "for( int i =0; i<%d;i++){\n", newseq->counts);
        fprintf(fptr, "  %s.ptr[i] = %s(%s.ptr[i]);\n", 
                node1->parent->rsibling->child->string,
                funcName->string,
                funcPara->child->string
                );

        fprintf(fptr, "}\n");

        break;
      }
      default:
        assert(0);// not implement.
        break; 
    }
    fprintf(fptr, "}\n");

  }else{ 
    // other cases 

    fprintf(fptr, "for(int i = 0, count =0; i<newseq.len; i++){\n");
    fprintf(fptr, "}\n");
  } 
  //fprintf(fptr, "for(int i = 0, count =0; i<%s.len; i++){\n");
}

void codegen(FILE *fptr, struct nodeType* node){

  struct nodeType *child = node->child;
      switch(node->nodeType){
        case NODE_NESL:{
          
          for(int i =0; i<node->counts ; i++){
            if(child->nodeType == NODE_DATATYPE){
              assert(0);// not implement;
            }
            child = child->rsibling;
          }
          for(int i =0 ; i< node-> counts ; i++ ){
            if(child->nodeType == NODE_FUNC){
              codegen(fptr, child);
            }
            child = child->rsibling;
          }
          fprintf(fptr, "int main(){\n");
          dumpTable(fptr,child);
          for(int i=0; i< node->counts; i++){
            if(child->nodeType !=NODE_FUNC && child->nodeType != NODE_DATATYPE)
              codegen(fptr, child);
          }
          fprintf(fptr, "\nreturn 0;\n}"); 
          break;
        }

        case NODE_FUNC:{
          struct SymTableEntry * entry = findSymbol(node->child->table, node->string);
          assert(entry);
          switch(entry->type){
            case TypeInt:
              fprintf(fptr, "int %s", node->string);
              codegen(fptr,node->child); //input parameter
              fprintf(fptr, "{\n");
              dumpTable(fptr, node->child);
    
              codegen(fptr,node->child->rsibling->rsibling);
              fprintf(fptr, "\n}\n");
              fprintf(fptr, "\n");
              break;
            case TypeSEQ_I:
              fprintf(fptr, "struct Sequence  %s", node->string);
              //codegen(fptr,node->child);
              printparam(fptr, node->child);
              fprintf(fptr, "{\nstruct Sequence res;\n");
              fprintf(fptr, "struct Sequence tmp;\n");
              
              // FIXME how to dump table without the inputparam
              //dumpTable(fptr, node->child);
              
              codegen(fptr, node->child->rsibling->rsibling);
              fprintf(fptr, "res = tmp;\nreturn res;\n");
              fprintf(fptr, "\n}\n");
              break;
            case TypeTuple:
              fprintf(fptr, "struct TypeTuple %s", node->string);
              codegen(fptr,node->child);
              fprintf(fptr, "{\n");
              fprintf(fptr, "{\nstruct TypeTuple res;\n");
              fprintf(fptr, "struct TypeTuple tmp;\n");
              codegen(fptr,node->child->rsibling->rsibling);
              fprintf(fptr, "\n}\n");
              break;
            default:
              assert(0);//not implement;
            break;
          }
          break;
        }
        
        case NODE_IFELSE:{
          struct nodeType* ifstmt = node->child;
          struct nodeType* thstmt = node->child->rsibling;
          struct nodeType* elstmt = node->child->rsibling->rsibling;
          codegen(fptr,ifstmt);
          codegen(fptr,thstmt);
          codegen(fptr,elstmt);
          break;
        }

        case NODE_IFSTMT:
          fprintf(fptr, "if");
          codegen(fptr, node->child);
          break;
        
        case NODE_THENSTMT:
          fprintf(fptr, "{\n");
          fprintf(fptr, "tmp = ");
          codegen(fptr, node->child);
          fprintf(fptr, ";\n}");
          break;

        case NODE_ELSESTMT:
          fprintf(fptr, "else{\n");
          codegen(fptr, node->child);
          fprintf(fptr, "}\n");
          break;
        
        case NODE_LET:
          dumpTable(fptr, node);
          codegen(fptr, node->child);
          fprintf(fptr, "\ntmp = ");
          codegen(fptr, node->child->rsibling);
          fprintf(fptr, ";\n");
          break;
        
        case NODE_BIND:{
          struct nodeType* child = node->child;
          if(child){
            do{
              codegen(fptr, child);
              child = child->rsibling;
            }while(child!=node->child);
          }  
          break;
        }
        case NODE_EXP:
        case NODE_PATTERN:
          codegen(fptr, node->child);
          break;
        case NODE_OP:
          switch(node->op){
            case OP_LT:
              codegen(fptr, node->child);
              fprintf(fptr, " < ");
              codegen(fptr, node->child->rsibling);
              break;
            
            case OP_GT:
              codegen(fptr, node->child);
              fprintf(fptr, " > ");
              codegen(fptr, node->child->rsibling);
              break;
            
            case OP_EQ:
              codegen(fptr, node->child);
              fprintf(fptr, " == ");
              codegen(fptr, node->child->rsibling);
              break;
            
            case OP_SHARP:
              codegen(fptr, node->child);
              fprintf(fptr, ".len");
              break;
            
            case OP_BIND:
              
              codegen(fptr, node->child);
              switch(node->child->rsibling->nodeType){
              case NODE_APPLYBODY1:
              case NODE_APPLYBODY2:
              case NODE_APPLYBODY3:
              case NODE_APPLYBODY4:
                fprintf(fptr, ".ptr = ");
                codegen(fptr, node->child->rsibling);
                break;
              default :
                fprintf(fptr, " = ");
                codegen(fptr, node->child->rsibling);
                fprintf(fptr, ";\n");
              }
              break;
            
            case OP_PP:
              fprintf(fptr, "concat(");
              codegen(fptr, node->child);
              fprintf(fptr, ",");
              codegen(fptr, node->child->rsibling);
              fprintf(fptr, ")");
              break;
            case OP_DIV:
              codegen(fptr, node->child);
              fprintf(fptr, "/");
              codegen(fptr, node->child->rsibling);
              break;
          }
          break;
       
        case NODE_SEQ_REF:
          codegen(fptr, node->child);
          fprintf(fptr, ".ptr[");
          codegen(fptr, node->child->rsibling);
          fprintf(fptr, "]");
          break;
        
        case NODE_APPLYBODY1:
          abort();
          break;
        
        case NODE_APPLYBODY2:
          codegen(fptr, node->child->rsibling);
          APP2printFor(fptr, node->child, node->child->rsibling);
          //fprintf(fptr, "for loop");
          break;
        
        case NODE_APPLYBODY3:
          codegen(fptr, node->child);
          APP3printFor(fptr, node->child, node->child->rsibling);
          //fprintf(fptr, "for loop");

          break;
        
        case NODE_APPLYBODY4:
          break;
        
        case NODE_RBINDS:
          //FIXME might have many children
          codegen(fptr, node->child);
          break;
        
        case NODE_IN:
          //FIXME here only consider the simple ApplyBody Exp.
          assert(node->child->nodeType == NODE_TOKEN);
          //assert(node->child->rsibling->nodeType == NODE_TOKEN);
          switch(node->child->rsibling->nodeType){
          case NODE_TOKEN:{
            struct SymTableEntry *entry = 
              findSymbol(node->table, node->child->rsibling->string);
            assert(entry);
            switch(entry->type){
              case TypeSEQ_I:
                fprintf(fptr, "malloc(sizeof(int)*%s.len);\n",entry->name);
                break;
              case TypeSEQ_F:
                fprintf(fptr, "malloc(sizeof(float)*%s.len);\n",entry->name);
                break;
              case TypeSEQ_B:
                fprintf(fptr, "malloc(sizeof(bool)*%s.len);\n",entry->name);
                break;
              case TypeSEQ_C:
                fprintf(fptr, "malloc(sizeof(char)*%s.len);\n",entry->name);
                break;
              case TypeSEQ:
                fprintf(fptr, "malloc(sizeof(%s.ptr[0])*%s.len);\n",
                                            entry->name,entry->name);
                break;
              default:
                assert(0); //break;
                break;
            }
            break;
          }
          
          case NODE_NEW_SEQ:
            switch(node->valueType){
              case TypeSEQ:
                fprintf(fptr, "malloc(sizeof(%s)*%d);\n",
                              node->child->rsibling->child->string,
                              node->child->rsibling->counts);
                //FIXME problems in the future.
                fprintf(fptr, "%s.len = %d;\n",
                              node->parent->parent->rsibling->child->string,
                              node->child->rsibling->counts);
                break;

            }
          break;
          case NODE_FUNC_CALL:
           break; 
          default:
            abort();
            break;
          }
          break;

        case NODE_TUPLE:
          codegen(fptr, node->child);
          fprintf(fptr, " , ");
          codegen(fptr, node->child->rsibling);
          break;

        case NODE_PAIR:
          fprintf(fptr, "(");
          assert(child);
          //codegen(fptr,node->child);
          do{
            codegen(fptr,node->child);
            child = child->rsibling;
          }while(child!=node->child);
          fprintf(fptr, ")");
          break;
        case NODE_INT:
          fprintf(fptr," %d ",node->iValue);
          break;

        case NODE_FUNC_CALL:{
          if(!strcmp(node->child->string, "rand")){
             fprintf(fptr, "rand"); 
          }else if(!strcmp(node->child->string, "dist")){
             fprintf(fptr, "dist"); 
          }else if(!strcmp(node->child->string, "time")){
             fprintf(fptr, "time"); 
          }else{
            codegen(fptr, node->child);
          }
          codegen(fptr, node->child->rsibling);
          break;
        }
        
        case NODE_TOKEN:
          switch(node->tokenType){
            case TOKE_ID:{
              // FIXME 0215 if I dump the table in the first, then I don't need
              // to worried whether here is the declaration or assignment.
              struct SymTableEntry* entry = findSymbol(node->table, node->string);
              assert(entry);
              fprintf(fptr, "%s", node->string);
            }
              
          }
          break;
        default:
          break;
      }
  return;
}

