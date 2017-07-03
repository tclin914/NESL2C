#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "node.h"
#include "codegen.h"
#include "symtab.h"
#include "codegencheck.h"
#include "sqcodegen.h"


struct RefTable refTable;
int issrand;

void sqcodegen(FILE *fptr, struct nodeType* node){
    struct nodeType *child = node->child;
    switch(node->nodeNum){
    case NODE_NESL:{

        for(int i =0; i<node->counts ; i++){
            if(child->nodeNum != NODE_DATATYPE && child->nodeNum!= NODE_FUNC){
                printGlobalVar(fptr, child); 
            }
            child = child->rsibling;
        }

        for(int i =0; i<node->counts ; i++){
            if(child->nodeNum == NODE_DATATYPE){
                assert(0);// not implement;
            }
            child = child->rsibling;
        }

        for(int i =0 ; i< node-> counts ; i++ ){
            if(child->nodeNum == NODE_FUNC){
                sqcodegen(fptr, child);
            }
            child = child->rsibling;
        }
        fprintf(fptr, "#pragma pf global parallel\n");
        fprintf(fptr, "void myFunc1(){\n");
        dumpTable(fptr, node);
        for(int i =0;i<node->counts;i++){
            if(child->nodeNum !=NODE_DATATYPE && child->nodeNum != NODE_FUNC){
                sqcodegen(fptr, child);
            }
            child = child->rsibling;
        }
        break;
    }

    case NODE_FUNC:{
        struct SymTableEntry * entry = findSymbol(node->child->table, node->string, REFERENCE);
        struct nodeType * parameter = node->child;
        struct nodeType * funcbody = parameter->rsibling->rsibling;

        assert(entry);
        if(node->isparallel_rr)
            fprintf(fptr, "#pragma pf device parallel\n");
        else 
            fprintf(fptr, "#pragma pf device inline\n");

        switch(entry->type){
        case TYPEINT:
            fprintf(fptr, "int %s", node->string);
            fprintf(fptr, "(");
            sqcodegen(fptr, parameter);
            fprintf(fptr, ")");
            fprintf(fptr, "{\nint _res;\n");
            dumpTable(fptr, parameter);
            sqcodegen(fptr, parameter);
            sqcodegen(fptr,funcbody);
            fprintf(fptr, "_res = %s;\n",funcbody->string);
            fprintf(fptr, "return _res;\n");
            fprintf(fptr, "\n}\n");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "float %s", node->string);
            fprintf(fptr, "(");
            sqcodegen(fptr, parameter);
            fprintf(fptr, ")");
            fprintf(fptr, "{\nfloat _res;\n");
            dumpTable(fptr, parameter);
            sqcodegen(fptr, parameter);
            sqcodegen(fptr,funcbody);
            fprintf(fptr, "_res = %s;\n",funcbody->string);
            fprintf(fptr, "return _res;\n");
            fprintf(fptr, "\n}\n");
            break;
        case TYPESEQ:
            fprintf(fptr, "struct Sequence  %s", node->string);
            fprintf(fptr, "(");
            sqcodegen(fptr, parameter);
            fprintf(fptr, ")");
            fprintf(fptr, "{\nstruct Sequence _res;\n");
            dumpTable(fptr, parameter);
            sqcodegen(fptr, parameter);
            sqcodegen(fptr, funcbody);
            fprintf(fptr, "_res = %s;\n",funcbody->string);
            fprintf(fptr, "return _res;\n");
            fprintf(fptr, "\n}\n");
            break;
        case TYPETUPLE:
            fprintf(fptr, "struct ");
            gentypes(fptr, entry->link->typeNode);
            fprintf(fptr, " %s", node->string);
            fprintf(fptr, "(");
            sqcodegen(fptr, parameter);
            fprintf(fptr, ")");
            fprintf(fptr, "{\nstruct "); 
            gentypes(fptr, entry->link); 
            fprintf(fptr, " _res;\n");
            dumpTable(fptr, parameter);
            sqcodegen(fptr, parameter);
            sqcodegen(fptr, funcbody);
            fprintf(fptr, "_res = %s;\n",funcbody->string);
            fprintf(fptr, "return _res;\n");
            fprintf(fptr, "\n}\n");
            break;
        default:
            assert(0);//not implement;
            break;
        }
        break;
    }
    case FPARAM_TOKEN:{
        if(!node->isvisited){
            struct SymTableEntry * entry;
            printtype(fptr, node); 
            assert(node->string);
            fprintf(fptr, " %s", node->string);
            entry = findSymbol(node->table, node->string, REFERENCE);
            assert(entry);
            entry ->isParam = 1;
            node->isvisited = 1;
        }else{
            ;//assert(0); 
        }
        break;
    }
    case FPARAM_TUPLE:{
        struct nodeType *LHS=node->child;
        struct nodeType *RHS=LHS->rsibling;
        if(!node->isvisited){
            struct SymTableEntry * entry;
            /* generate function prototype */
            printtype(fptr, node); 
            assert(node->string);
            fprintf(fptr, " %s", node->string);
            //entry = findSymbol(rootTable, node->string, REFERENCE);
            //assert(entry);
            //entry ->isParam = 1;
            node->isvisited = 1;
        }
        else{
            /* bind the original var with tuple node.
             ** linked by node->functioncall in use node->tuplenode as reference.
             ** tuplenode is linked with the original variables in codegencheck pass.
             */

            /* if child is token then we assign it with member of tuple. */
            switch(LHS->nodeNum){
            case FPARAM_TUPLE:
                fprintf(fptr, "%s = %s.a;\n",LHS->string, node->string);
                LHS->isvisited=1;
                sqcodegen(fptr,LHS);
                break;
            case NODE_TOKEN:
                fprintf(fptr, "%s = %s.a;\n",LHS->string, node->string);
                break;
            default:
                assert(0);
                break;
            }
            switch(RHS->nodeNum){
            case FPARAM_TUPLE:
                fprintf(fptr, "%s = %s.b;\n",RHS->string, node->string);
                RHS->isvisited=1;
                sqcodegen(fptr,RHS);
                break;
            case NODE_TOKEN:
                fprintf(fptr, "%s = %s.b;\n",RHS->string, node->string);
                break;
            default:
                assert(0);
                break;
            } 
        }

        break;
    }
    case NODE_IFELSE:{
        struct nodeType* ifstmt = node->child;
        struct nodeType* thstmt = node->child->rsibling;
        struct nodeType* elstmt = node->child->rsibling->rsibling;
        sqcodegen(fptr,ifstmt);
        sqcodegen(fptr,thstmt);
        sqcodegen(fptr,elstmt);
        ////DECREF(fptr,refTable.size);
        break;
    }

    case NODE_IFSTMT:{
        struct nodeType * condition = node->child;
        while(condition->nodeNum == NODE_PAIR) condition = condition->child;
        sqcodegen(fptr, condition); 
        fprintf(fptr, "if(%s)",condition->string);
        break;
    }
    case NODE_ELSESTMT:
        fprintf(fptr, "else");
    case NODE_THENSTMT:{
        struct nodeType* LHS= node->child;
        fprintf(fptr, "{\n");
        switch(LHS->nodeNum){
        case NODE_TOKEN:
            assert(LHS->string);
            fprintf(fptr, "%s = %s;\n",node->string, LHS->string);
            break;
        case NODE_INT:
            fprintf(fptr, "%s = %d;\n", node->string,LHS->iValue);
            break;
        case NODE_FLOAT:
            fprintf(fptr, "%s = %f;\n", node->string,LHS->rValue);
            break;
        case NODE_CHAR:
            fprintf(fptr, "%s = %s;\n", node->string,LHS->string);
            break;
        case NODE_BOOL:
            fprintf(fptr, "%s = %i;\n", node->string,LHS->iValue);
            break;
        default:
            sqcodegen(fptr, LHS);
            assert(LHS->string);
            fprintf(fptr, "%s = %s;\n", node->string,LHS->string);
            break;
        }
        break;}

    case NODE_LET:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = node->child->rsibling;
        //    int refaddcount=refTable.size;
        fprintf(fptr, "{\n");

        dumpTable(fptr, node);
        sqcodegen(fptr, LHS);
        sqcodegen(fptr, RHS);
        assert(RHS->string);
        //    assert(refTable.size>=refaddcount);
        //    refaddcount = refTable.size-refaddcount;
        //#ifdef DEBUG
        //    fprintf(fptr,"//DEBUG LETrefaddcount:%d\n",refaddcount);
        //#endif
        //DECREF(fptr,refaddcount);
        fprintf(fptr, "//end of LET\n");

        break;}//end of LET
    case NODE_LETRET:{
        struct nodeType *LHS = node->child;
        struct RefTableEntry *entry;
        int index;
        //    int refaddcount=refTable.size;
        while (LHS->nodeNum == NODE_PAIR) LHS = LHS->child;
        switch(LHS->nodeNum){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
            break;
        default:

            sqcodegen(fptr,LHS);
            break;
        }
        assert(LHS->string);
        fprintf(fptr, "%s = %s;\n", node->string,LHS->string);
        //    index = findREF(LHS->string);
        //    if(index!=-1){
        //      entry = &refTable.entries[index];
        //      assert(LHS == entry->link);
        //      deleteREF(index, index+1);
        //    }
        //    refaddcount = refTable.size - refaddcount;
        //#ifdef DEBUG
        //    fprintf(fptr,"//DEBUG LETRETrefaddcount:%d\n",refaddcount);
        //#endif
        //DECREF(fptr,refaddcount);
        fprintf(fptr, "//end of LETRET\n");

        break; } // end of LETRET
    case NODE_BIND:{
        struct nodeType* child = node->child;
        if(child){
            do{
                sqcodegen(fptr, child);
                child = child->rsibling;
            }while(child!=node->child);
        }  
        break;
    } // end of NODE_BIND
    case NODE_EXP:
    case NODE_PATTERN:
        sqcodegen(fptr, node->child);
        break;
    case NODE_ASSIGN:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = node->child->rsibling;

        switch(RHS->nodeNum){
        case NODE_APPLYBODY1:
        case NODE_APPLYBODY2:
        case NODE_APPLYBODY3:
        case NODE_APPLYBODY4:
        case NODE_FUNC_CALL:
            sqcodegen(fptr, RHS);
            break;
        case NODE_PAIR:{
            while(RHS->nodeNum == NODE_PAIR) RHS=RHS->child;
            sqcodegen(fptr, RHS);
            RHS= LHS->rsibling;
            break;}
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
            break;
        case NODE_TUPLE:
            assert(0); // not likely happened.
            break;
        default :
            sqcodegen(fptr, RHS);
            assert(RHS->string);
            break;
        }// end of RHS->nodeNum;

        switch(LHS->nodeNum){
        case NODE_TOKEN:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
            sqcodegen(fptr, LHS);
            break;
        case NODE_PATTERN:{
            //FIXME pattern in document is complicated.
            int i=0;
            while(!(LHS->string)&&i<=10) {LHS = LHS->child;i++;}
            fprintf(fptr, "%s",LHS->string);
            LHS= node->child;
            break;}
        case NODE_PAIR:{
            while(LHS->nodeNum == NODE_PAIR) LHS = LHS->child;
            fprintf(fptr, "%s",LHS->string);
            LHS= LHS->rsibling;
            break;}
        default:
            fprintf(fptr, "%s",LHS->string);
            break;
        }
        LHS = node->child;

        fprintf(fptr, " = ");

        switch (RHS->nodeNum){
        case NODE_TOKEN:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
            sqcodegen(fptr, RHS);
            break;
        default:
            while(RHS->nodeNum == NODE_PAIR) RHS = RHS->child;
            assert(RHS->string);
            fprintf(fptr, "%s",RHS->string);
            RHS= LHS->rsibling;
        }
        fprintf(fptr, ";\n");
        if(RHS->dataType.type == TYPETUPLE){

        }
        while(LHS->nodeNum == NODE_PATTERN) LHS=LHS->child;
        while(LHS->nodeNum ==NODE_PAIR) LHS=LHS->child;
        if(LHS->nodeNum == LHS_TUPLE||LHS->nodeNum == NODE_TUPLE){
            sqcodegen(fptr, LHS);
        }

        fprintf(fptr,"//end of ASSIGNMENT\n");
        break;
    }// end of NODE_ASSIGN;

    case NODE_OP:{
        switch(node->op){
        case OP_LT:
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " < ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;

                /* remove pairs */
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;

                /*open new scope*/ 
                fprintf(fptr,"{\n");

                /*declare op1*/
                switch(op1->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                default:
                    assert(0);
                }

                /* declare op2*/
                switch(op2->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op2;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op2;\n");
                    break;
                default:
                    assert(0);
                }

                /*assign op1*/
                switch(op1->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op1 = %d;\n", op1->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op1 = %f;\n", op1->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op1 = %s;\n", op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;  
                }

                /*assign op2*/
                switch(op2->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op2 = %d;\n", op2->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op2 = %f;\n", op2->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op2 = %s;\n", op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr,op2);
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;  
                }

                fprintf(fptr, "%s = op1 < op2;\n",node->string);

                /*close scope*/
                fprintf(fptr,"}\n"); 
            }

            break;
        case OP_GT:
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " > ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;

                /* remove pairs */
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;

                /*open new scope*/ 
                fprintf(fptr,"{\n");

                /*declare op1*/
                switch(op1->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                default:
                    assert(0);
                }

                /* declare op2*/
                switch(op2->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op2;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op2;\n");
                    break;
                default:
                    assert(0);
                }

                /*assign op1*/
                switch(op1->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op1 = %d;\n", op1->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op1 = %f;\n", op1->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op1 = %s;\n", op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;  
                }

                /*assign op2*/
                switch(op2->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op2 = %d;\n", op2->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op2 = %f;\n", op2->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op2 = %s;\n", op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr,op2);
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;  
                }

                fprintf(fptr, "%s = op1 > op2;\n",node->string);

                /*close scope*/
                fprintf(fptr,"}\n"); 
            }

            break;

        case OP_LE:
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " <= ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;

                /* remove pairs */
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;

                /*open new scope*/ 
                fprintf(fptr,"{\n");

                /*declare op1*/
                switch(op1->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                default:
                    assert(0);
                }

                /* declare op2*/
                switch(op2->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op2;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op2;\n");
                    break;
                default:
                    assert(0);
                }

                /*assign op1*/
                switch(op1->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op1 = %d;\n", op1->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op1 = %f;\n", op1->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op1 = %s;\n", op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;  
                }

                /*assign op2*/
                switch(op2->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op2 = %d;\n", op2->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op2 = %f;\n", op2->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op2 = %s;\n", op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr,op2);
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;  
                }

                fprintf(fptr, "%s = op1 <= op2;\n",node->string);

                /*close scope*/
                fprintf(fptr,"}\n"); 
            }

            break;
        case OP_GE:
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " >= ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;

                /* remove pairs */
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;

                /*open new scope*/ 
                fprintf(fptr,"{\n");

                /*declare op1*/
                switch(op1->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                default:
                    assert(0);
                }

                /* declare op2*/
                switch(op2->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op2;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op2;\n");
                    break;
                default:
                    assert(0);
                }

                /*assign op1*/
                switch(op1->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op1 = %d;\n", op1->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op1 = %f;\n", op1->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op1 = %s;\n", op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;  
                }

                /*assign op2*/
                switch(op2->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op2 = %d;\n", op2->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op2 = %f;\n", op2->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op2 = %s;\n", op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr,op2);
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;  
                }

                fprintf(fptr, "%s = op1 >= op2;\n",node->string);

                /*close scope*/
                fprintf(fptr,"}\n"); 
            }

            break;
        case OP_NE:
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " != ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;

                /* remove pairs */
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;

                /*open new scope*/ 
                fprintf(fptr,"{\n");

                /*declare op1*/
                switch(op1->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                default:
                    assert(0);
                }

                /* declare op2*/
                switch(op2->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op2;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op2;\n");
                    break;
                default:
                    assert(0);
                }

                /*assign op1*/
                switch(op1->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op1 = %d;\n", op1->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op1 = %f;\n", op1->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op1 = %s;\n", op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;  
                }

                /*assign op2*/
                switch(op2->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op2 = %d;\n", op2->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op2 = %f;\n", op2->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op2 = %s;\n", op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr,op2);
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;  
                }

                fprintf(fptr, "%s = op1 != op2;\n",node->string);

                /*close scope*/
                fprintf(fptr,"}\n"); 
            }

            break;
        case OP_EQ:
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " == ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;

                /* remove pairs */
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;

                /*open new scope*/ 
                fprintf(fptr,"{\n");

                /*declare op1*/
                switch(op1->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                default:
                    assert(0);
                }

                /* declare op2*/
                switch(op2->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op2;\n");
                    break;
                case TYPECHAR:
                    fprintf(fptr, "char op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op2;\n");
                    break;
                default:
                    assert(0);
                }

                /*assign op1*/
                switch(op1->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op1 = %d;\n", op1->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op1 = %f;\n", op1->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op1 = %s;\n", op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1 = %s;\n",op1->string);
                    break;  
                }

                /*assign op2*/
                switch(op2->nodeNum){
                case NODE_TOKEN:
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;
                case NODE_INT:
                    fprintf(fptr, "op2 = %d;\n", op2->iValue);
                    break;
                case NODE_FLOAT:
                    fprintf(fptr, "op2 = %f;\n", op2->rValue);
                    break;
                case NODE_CHAR:
                    fprintf(fptr, "op2 = %s;\n", op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr,op2);
                    fprintf(fptr, "op2 = %s;\n",op2->string);
                    break;  
                }

                fprintf(fptr, "%s = op1 == op2;\n",node->string);

                /*close scope*/
                fprintf(fptr,"}\n"); 
            }

            break;
        case OP_ADD:{
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " + ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;
                fprintf(fptr,"{\n");
                printparam(fptr, node);// declare int add; etc...
                switch(node->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1,op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1,op2;\n");
                    break;
                }
                switch(op1->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op1=");
                    sqcodegen(fptr, op1);
                    fprintf(fptr, ";\n");
                    break;
                    break;
                case NODE_TOKEN:
                    assert(op1->string);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                }
                switch(op2->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op2=");
                    sqcodegen(fptr, op2);
                    fprintf(fptr, ";\n");
                    break;
                case NODE_TOKEN:
                    assert(op2->string);
                    fprintf(fptr, "op2=%s;\n",op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr, op2);
                    fprintf(fptr, "op2=%s;\n", op2->string);
                    break;
                }
                fprintf(fptr, "%s = op1 + op2;\n",node->string);
                fprintf(fptr,"}\n");
            }
            break;
        }
        case OP_SUB:{
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " - ");
                sqcodegen(fptr, node->child->rsibling);
            }else{ 
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;
                fprintf(fptr,"{\n");
                printparam(fptr, node);// declare int add; etc...
                switch(node->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1,op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1,op2;\n");
                    break;
                }
                switch(op1->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op1=");
                    sqcodegen(fptr, op1);
                    fprintf(fptr, ";\n");
                    break;
                    break;
                case NODE_TOKEN:
                    assert(op1->string);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                }
                switch(op2->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op2=");
                    sqcodegen(fptr, op2);
                    fprintf(fptr, ";\n");
                    break;
                case NODE_TOKEN:
                    assert(op2->string);
                    fprintf(fptr, "op2=%s;\n",op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr, op2);
                    fprintf(fptr, "op2=%s;\n", op2->string);
                    break;
                }
                fprintf(fptr, "%s = op1 - op2;\n",node->string);
                fprintf(fptr,"}\n");
            }
            break;
        }
        case OP_MUL:{
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " * ");
                sqcodegen(fptr, node->child->rsibling);
            }else{

                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;
                fprintf(fptr,"{\n");
                printparam(fptr, node);// declare int add; etc...
                switch(node->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1,op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1,op2;\n");
                    break;
                }
                switch(op1->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op1=");
                    sqcodegen(fptr, op1);
                    fprintf(fptr, ";\n");
                    break;
                    break;
                case NODE_TOKEN:
                    assert(op1->string);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                }
                switch(op2->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op2=");
                    sqcodegen(fptr, op2);
                    fprintf(fptr, ";\n");
                    break;
                case NODE_TOKEN:
                    assert(op2->string);
                    fprintf(fptr, "op2=%s;\n",op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr, op2);
                    fprintf(fptr, "op2=%s;\n", op2->string);
                    break;
                }
                fprintf(fptr, "%s = op1 * op2;\n",node->string);
                fprintf(fptr,"}\n");
            }
            break;
        }    
        case OP_DIV:{
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " / ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;
                fprintf(fptr,"{\n");
                printparam(fptr, node);// declare int add; etc...
                switch(node->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1,op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1,op2;\n");
                    break;
                }
                switch(op1->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op1=");
                    sqcodegen(fptr, op1);
                    fprintf(fptr, ";\n");
                    break;
                    break;
                case NODE_TOKEN:
                    assert(op1->string);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                }
                switch(op2->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op2=");
                    sqcodegen(fptr, op2);
                    fprintf(fptr, ";\n");
                    break;
                case NODE_TOKEN:
                    assert(op2->string);
                    fprintf(fptr, "op2=%s;\n",op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr, op2);
                    fprintf(fptr, "op2=%s;\n", op2->string);
                    break;
                }
                fprintf(fptr, "%s = op1 / op2;\n",node->string);
                fprintf(fptr,"}\n");
            }
            break;
        } 
        case OP_UMINUS:{
            if(node->infilter){
                sqcodegen(fptr, node->child);
                fprintf(fptr, " - ");
                sqcodegen(fptr, node->child->rsibling);
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;
                fprintf(fptr,"{\n");
                printparam(fptr, node);// declare int add; etc...
                switch(node->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1;\n");
                    break;
                }
                switch(op1->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op1=");
                    sqcodegen(fptr, op1);
                    fprintf(fptr, ";\n");
                    break;
                    break;
                case NODE_TOKEN:
                    assert(op1->string);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                }

                fprintf(fptr, "%s = -op1;\n",node->string);
                fprintf(fptr,"}\n");
            }
            break;
        }
        case OP_UPT:{
            struct nodeType *op1 = node->child;
            struct nodeType *op2 = node->child->rsibling;
            assert(op1->dataType.type ==TYPEFLOAT||op1->dataType.type ==TYPEFLOAT);
            assert(op2->dataType.type ==TYPEINT);
            if(node->infilter){
                fprintf(fptr, " pow( ");
                sqcodegen(fptr, node->child);
                fprintf(fptr, " , ");
                sqcodegen(fptr, node->child->rsibling);
                fprintf(fptr, " )");
            }else{
                struct nodeType *op1 = node->child;
                struct nodeType *op2 = op1->rsibling;
                while(op1->nodeNum == NODE_PAIR) op1=op1->child;
                while(op2->nodeNum == NODE_PAIR) op2=op2->child;
                fprintf(fptr,"{\n");
                printparam(fptr, node);// declare int add; etc...
                switch(node->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "int op1,op2;\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float op1,op2;\n");
                    break;
                }
                switch(op1->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op1=");
                    sqcodegen(fptr, op1);
                    fprintf(fptr, ";\n");
                    break;
                    break;
                case NODE_TOKEN:
                    assert(op1->string);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                default:
                    assert(op1->string);
                    sqcodegen(fptr, op1);
                    fprintf(fptr, "op1=%s;\n",op1->string);
                    break;
                }
                switch(op2->nodeNum){
                case NODE_INT:
                case NODE_FLOAT:
                    fprintf(fptr, "op2=");
                    sqcodegen(fptr, op2);
                    fprintf(fptr, ";\n");
                    break;
                case NODE_TOKEN:
                    assert(op2->string);
                    fprintf(fptr, "op2=%s;\n",op2->string);
                    break;
                default:
                    assert(op2->string);
                    sqcodegen(fptr, op2);
                    fprintf(fptr, "op2=%s;\n", op2->string);
                    break;
                }
                fprintf(fptr, "%s=",node->string);
                if(op1->dataType.type == TYPEINT) fprintf(fptr, "(int)");
                fprintf(fptr, " pow((float)op1, op2);\n");
                fprintf(fptr,"}\n");

            }
            break;
        }
        case OP_SHARP:{
            struct nodeType *op1 = node->child;
            while(op1->nodeNum == NODE_PAIR) op1=op1->child;
            fprintf(fptr,"{\n");
            assert(op1->dataType.type==TYPESEQ); 
            switch(op1->nodeNum){
            case NODE_TOKEN:
                assert(op1->string);
                fprintf(fptr, "%s=%s.len;\n",node->string,op1->string);
                break;
            default:
                assert(op1->string);
                sqcodegen(fptr, op1);
                fprintf(fptr, "%s=%s.len;\n",node->string,op1->string);
                break;
            }

            fprintf(fptr,"}\n");
            break;
        }
        case OP_PP:
            // if child is a variable, it will be process here.
            if(node->child->nodeNum!=NODE_TOKEN&&node->child->nodeNum!=NODE_SEQ)
                sqcodegen(fptr, node->child);
            if(node->child->rsibling->nodeNum!=NODE_TOKEN)
                sqcodegen(fptr, node->child->rsibling);

            if(node->child->nodeNum == NODE_SEQ){
                fprintf(fptr, "PREPEND(%s, %s, %s, ",
                        node->child->child->string,
                        node->child->rsibling->string,
                        node->string);

            }else{
                fprintf(fptr, "CONCAT(%s, %s, %s, ",
                        node->child->string,
                        node->child->rsibling->string,
                        node->string);
            }
            switch(node->dataType.type){
            case TYPESEQ:
                switch(node->typeNode->child->dataType.type){
                case TYPETUPLE:
                    printtype(fptr, node->typeNode->child);
                    fprintf(fptr, ", ");
                    gentypes(fptr, node->typeNode->child);
                    fprintf(fptr, ");\n");
                    //fprintf(fptr, "struct tuple, tuple);\n");
                    break;
                case TYPEINT:
                    fprintf(fptr, "int , I);\n");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "float , F);\n");
                    break;
                default:
                    assert(0);
                }
                break;
            default:
                assert(0);//not implemented.
                break;
            }

            //FIXME problem in quicksort?  
            if(node->child->nodeNum!=NODE_SEQ)
                ;//printAddREF(fptr, node->string, node->dataType.type, node);
            break; // end of OP_PP
        default:
            assert(0);// not implemented;
        }// end of switch(node->op)
        break;
    }// end of NODE_OP

    case NODE_SEQ_REF:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = node->child->rsibling;

        // generate the RHS's value if needed.
        switch (RHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            break;
        case NODE_PAIR:
            while(RHS->nodeNum == NODE_PAIR) RHS = RHS->child;
            sqcodegen(fptr, RHS);
            RHS= LHS->rsibling;
            break;
        default:
            sqcodegen(fptr, RHS);
            assert(RHS->string);
            break;
        }

        if(RHS->nodeNum == NODE_INT){ // if RHS is simply a number
            switch(node->dataType.type){
            case TYPEFLOAT:
                fprintf(fptr, "GET_ELEM_F(%s,%s,%d",node->string, LHS->string,RHS->iValue);
                fprintf(fptr, ");\n");
                break;
            case TYPEINT:
                fprintf(fptr, "GET_ELEM_I(%s,%s,%d",node->string, LHS->string,RHS->iValue);
                fprintf(fptr, ");\n");
                break;
            case TYPESEQ:
                //TODO
                fprintf(fptr, "GET_ELEM_SEQ");
                switch(node->typeNode->child->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "_I");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "_F");
                    break;
                default: 
                    assert(0);
                } 
                fprintf(fptr, "(%s,%s,%d",node->string, LHS->string,RHS->iValue);

                fprintf(fptr, ");\n");
                break;
            default:
                assert(0);
            }
        }else{  // if RHS is not simply a number.
            //sqcodegen(fptr, RHS); // print index.
            switch(node->dataType.type){
            case TYPEINT:
                fprintf(fptr, "GET_ELEM_I(%s,%s,%s",node->string, LHS->string,RHS->string);
                fprintf(fptr, ");\n");
                break;
            case TYPESEQ:
                fprintf(fptr, "GET_ELEM_SEQ");
                switch(node->typeNode->child->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "_I");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "_F");
                    break;
                default: 
                    assert(0);
                } 
                fprintf(fptr, "(%s,%s,%s",node->string, LHS->string,RHS->string);
                fprintf(fptr, ");\n");
                break;
            case TYPETUPLE:
                fprintf(fptr, "GET_ELEM_");
                gentypes(fptr, node);
                fprintf(fptr, "(%s,%s,%s",node->string, LHS->string, RHS->string);
                fprintf(fptr, ");\n");
                break;
            default:
                assert(0);
            }
        }
        break;
    }
    case GEN_SEQ_REF:
        //sqcodegen(fptr, node->child);
        switch(node->dataType.type){
        case TYPEINT:
            fprintf(fptr, "GET_ELEM_I(%s,%s,",node->string, node->child->rsibling->child->string);
            sqcodegen(fptr, node->child->rsibling->child->rsibling); // print index.
            fprintf(fptr, ");\n");
            break;
        default:
            assert(0);//not implemented.
            break;
        }
        break;
    case PARAM_TUPLE:{
        struct nodeType *LHS=node->child;
        struct nodeType *RHS = node->child->rsibling;
        struct nodeType *child = node->child;

        if(!node->isvisited){ // first time
            do{
                switch(child->nodeNum){
                case NODE_TOKEN:
                case NODE_INT:
                case NODE_FLOAT:
                case NODE_CHAR:
                case NODE_BOOL:
                    break;
                case NODE_PAIR:{
                    struct nodeType *tmp = child;
                    while(tmp->nodeNum == NODE_PAIR) tmp = tmp->child;
                    sqcodegen(fptr, tmp);
                    assert(tmp->string);
                    child->string = malloc(sizeof(char)*100);
                    strcpy(child->string, tmp->string);
                    //free(tmp->string);
                    child->dataType.type = tmp->dataType.type;
                    child->typeNode = tmp->typeNode;
                    break;}
                case PARAM_TUPLE:
                    sqcodegen(fptr,child);
                    break;
                default:
                    sqcodegen(fptr,child); 
                    if(child->nodeNum!=NODE_PAIR)
                        assert(child->string);
                    break;
                }
                child=child->rsibling;
            }while(child!=node->child);
            node->isvisited=1;

            //}else{ 
            // second time 
        while(LHS->nodeNum == NODE_PAIR) LHS= LHS->child;
        fprintf(fptr, "%s.a = ",node->string);
        switch(LHS->nodeNum){
        case NODE_TOKEN:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
            sqcodegen(fptr,LHS);
            break;
        default:
            fprintf(fptr,"%s",LHS->string);
            break;
        }
        LHS= node->child;
        fprintf(fptr, ";\n");
        fprintf(fptr, "%s.b = ",node->string);
        while(RHS->nodeNum == NODE_PAIR) RHS= RHS->child;
        switch(RHS->nodeNum){
        case NODE_TOKEN:
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
            sqcodegen(fptr,RHS);
            break;
        default:
            fprintf(fptr,"%s",RHS->string);
            break;
        }
        RHS= node->child->rsibling;
        fprintf(fptr, ";\n ");
    }// end of if else

        break;
    }//end of PARAM_TUPLE
    case NODE_SEQ_TUPLE:{
        struct nodeType *LHS=node->child;
        struct nodeType *RHS = node->child->rsibling;
        switch(LHS->nodeNum){
        case NODE_INT:
            fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n",
                    LHS->iValue, node->string, LHS->paramcount);
            break;
        case NODE_FLOAT:
            fprintf(fptr, "SET_ELEM_F(%f,%s,%d);\n",
                    LHS->rValue, node->string, LHS->paramcount);
            break;
        default:
            sqcodegen(fptr, LHS);
            switch(LHS->dataType.type){
            case TYPEINT:
                fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                break;
            case TYPEFLOAT:
                fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                break;
                //case TYPETUPLE_I:
                //  fprintf(fptr, "SET_ELEM_PAIR_I(%s,%s,%d);\n",
                //      LHS->string, node->string, LHS->paramcount);
                //break;
            case TYPETUPLE:
                fprintf(fptr, "SET_ELEM_");
                gentypes(fptr, LHS);
                fprintf(fptr, "(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                //  fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
                //      LHS->string, node->string, LHS->paramcount);
                break;
            default:
                assert(0);
                break;

            }
            break;
            //assert(0);
        }
        switch(RHS->nodeNum){
        case NODE_SEQ_TUPLE:
            sqcodegen(fptr, RHS);
            break;
        case NODE_INT:
            fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n",
                    RHS->iValue, node->string, RHS->paramcount);
            break;
        default:
            sqcodegen(fptr, RHS);
            switch(RHS->dataType.type){
            case TYPEINT:
                fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                break;
            case TYPEFLOAT:
                fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                break;
            case TYPETUPLE:
                //TODO
                fprintf(fptr, "SET_ELEM_");
                gentypes(fptr, RHS);
                fprintf(fptr, "(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                break;
            default:
                assert(0);

            }    
        }
        break;
    }// end of NODE_SEQ_TUPLE

    case NODE_SEQ:{
        struct nodeType *LHS = node->child;
        int deccount =0; 

        switch(LHS->nodeNum){
        case NODE_INT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_FLOAT:
        case NODE_TOKEN:
            break;
        default:
            while(LHS->nodeNum == NODE_PAIR) LHS=LHS->child;
            sqcodegen(fptr,LHS);
            assert(LHS->string);
            LHS = node->child;
            break;
        }

        fprintf(fptr, "MALLOC(%s, 1,",node->string);

        switch(LHS->dataType.type){
        case TYPEINT:
            fprintf(fptr, "int");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "float");
            break;
        case TYPEBOOL:
            fprintf(fptr, "bool");
            break;
        case TYPECHAR:
            fprintf(fptr, "char");
            break;
        case TYPESEQ:
            fprintf(fptr, "struct Sequence");
            break;
        case TYPETUPLE:
            fprintf(fptr, "struct t_");
            assert(0);// not implement;
            break;
        default:
            assert(0);
            break;
        }
        fprintf(fptr,");\n");

        /* SET_ELEMENT */
        //switch(node->typeNode->dataType.type){
        //case TYPETUPLE:
        //    //TODO
            fprintf(fptr, "SET_ELEM_");
            gentypes(fptr, node->child);
            fprintf(fptr, "(");
        //    break;
        //default: 
        //    assert(0);
        //    break;
        //}
        
        switch(LHS->nodeNum){
        case NODE_INT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_FLOAT:
        case NODE_TOKEN:
            sqcodegen(fptr, LHS);
            break;
        default:
            while(LHS->nodeNum == NODE_PAIR) LHS=LHS->child;
            fprintf(fptr, "%s",LHS->string);
            LHS = node->child;
            break;
        }
        fprintf(fptr, ",%s,0);\n",node->string);
        break;
    }// end of NODE_SEQ

    case NODE_NEW_SEQ:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = node->child->rsibling;
        int deccount =0; 
        while(LHS->nodeNum == NODE_PAIR) LHS=LHS->child;
        while(RHS->nodeNum == NODE_PAIR) RHS=RHS->child;
        fprintf(fptr, "MALLOC(%s,%d,",node->string, node->counts);

        switch(LHS->dataType.type){
        case TYPEINT:
            fprintf(fptr, "int");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "float");
            break;
        case TYPEBOOL:
            fprintf(fptr, "bool");
            break;
        case TYPECHAR:
            fprintf(fptr, "char");
            break;
        case TYPESEQ:
            fprintf(fptr, "struct Sequence");
            break;
        case TYPETUPLE:
            printtype(fptr, LHS);
            //fprintf(fptr, "s`truct tuple");
            break;
        default:
            assert(0);
            break;
        }
        fprintf(fptr,");\n");


        switch(LHS->nodeNum){
        case NODE_INT:
            fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", LHS->iValue, node->string, LHS->paramcount);
            break;
        case NODE_FLOAT:
            fprintf(fptr, "SET_ELEM_F(%f,%s,%d);\n", LHS->rValue, node->string, LHS->paramcount); 
            break;
        case NODE_TOKEN:
            switch(LHS->dataType.type){
            case TYPESEQ:
                fprintf(fptr,"SET_ELEM_SEQ"); 
                switch(LHS->typeNode->child->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "_I");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "_F");
                    break;
                default:
                    assert(0);
                }
                fprintf(fptr,"(%s,%s,%d);\n", 
                        LHS->string, node->string, LHS->paramcount);
                break;
            case TYPETUPLE:
                fprintf(fptr, "SET_ELEM_");
                gentypes(fptr, LHS);
                fprintf(fptr, "(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
                //        LHS->string, node->string, LHS->paramcount);
                break;
            default:
                assert(0);
                break;
            }
            break;
        default:
            sqcodegen(fptr, LHS);
            switch(LHS->dataType.type){
            case TYPEINT:
                fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                break;
            case TYPEFLOAT:
                fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                break;
            case TYPETUPLE:
                fprintf(fptr, "SET_ELEM_");
                gentypes(fptr, LHS);
                fprintf(fptr, "(%s,%s,%d);\n",
                        LHS->string, node->string, LHS->paramcount);
                //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
                //        LHS->string, node->string, LHS->paramcount);
                break;
            default:
                assert(0);
            }
            break;
        }
        switch(RHS->nodeNum){
        case NODE_INT:
            fprintf(fptr, "SET_ELEM_I(%d,%s,%d);\n", RHS->iValue, node->string, RHS->paramcount); 
            break;
        case NODE_FLOAT:
            fprintf(fptr, "SET_ELEM_F(%f,%s,%d);\n", RHS->rValue, node->string, RHS->paramcount); 
            break;
        case NODE_TOKEN:
            switch(RHS->dataType.type){
            case TYPESEQ:
                fprintf(fptr,"SET_ELEM_SEQ"); 
                switch(LHS->typeNode->child->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "_I");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "_F");
                    break;
                default:
                    assert(0);
                }
                fprintf(fptr,"(%s,%s,%d);\n", 
                        RHS->string, node->string, RHS->paramcount);
                break;
            case TYPETUPLE:
                fprintf(fptr, "SET_ELEM_");
                gentypes(fptr, RHS);
                fprintf(fptr, "(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
                //        RHS->string, node->string, RHS->paramcount);
                break;
            default:
                assert(0);
                break;
            }
            break;
        case NODE_SEQ_TUPLE:
            sqcodegen(fptr, RHS);
            break;
        default:
            sqcodegen(fptr, RHS);
            switch(RHS->dataType.type){
            case TYPEINT:
                fprintf(fptr, "SET_ELEM_I(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                break;
            case TYPEFLOAT:
                fprintf(fptr, "SET_ELEM_F(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                break;
            case TYPETUPLE:
                fprintf(fptr, "SET_ELEM_");
                gentypes(fptr, RHS);
                fprintf(fptr, "(%s,%s,%d);\n",
                        RHS->string, node->string, RHS->paramcount);
                //fprintf(fptr, "SET_ELEM_PAIR(%s,%s,%d);\n",
                //        RHS->string, node->string, RHS->paramcount);
                break;
            default:
                assert(0);
            }
            break;
        }

        break;
    }// end of NODE_NEW_SEQ

    case NODE_APPLYBODY1:{
        struct nodeType *LHS = node->child;
        struct nodeType *RBINDS = LHS;
        struct nodeType *rbchild = RBINDS->child;
        struct nodeType *varchild = rbchild->child;
        struct nodeType *arrchild = varchild->rsibling;
        int refaddcount=0;
        int forlooprefaddcount=0;

        //open scope
        fprintf(fptr, "{\n");
        dumpTable(fptr, node);

        /*generate the src arrays.*/
        refaddcount = refTable.size;
        //sqcodegen(fptr, SRCARR);
        do{
            arrchild = rbchild->child->rsibling;
            if(arrchild->nodeNum!=NODE_TOKEN)
                sqcodegen(fptr, arrchild);
            rbchild = rbchild->rsibling;
        }while(rbchild!=RBINDS->child);

        refaddcount = refTable.size-refaddcount; 

        /* allocate the dest array.*/
        fprintf(fptr, "MALLOC(%s,%s.len,",node->string, arrchild->string);

        switch(varchild->dataType.type){
        case TYPEINT:
            fprintf(fptr, "int");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "float");
            break;
        case TYPEBOOL:
            fprintf(fptr, "bool");
            break;
        case TYPECHAR:
            fprintf(fptr, "char");
            break;
        case TYPESEQ:
            fprintf(fptr, "struct Sequence");
            break;
        case TYPETUPLE:
            fprintf(fptr, "struct ");
            gentypes(fptr,LHS);
            break;
        default:
            assert(0);
            break;
        }
        fprintf(fptr,");\n");

        /* generate for loop */
        forlooprefaddcount = refTable.size;
        fprintf(fptr, "_len = %s.len;\n",node->string);
        fprintf(fptr, "#pragma pf parallel_rr\n");
        fprintf(fptr, "for (_i =0; _i <_len;_i++){\n");
        dumpTable(fptr, node->child); 
        /* get elem from src array.*/
        do{
            varchild = rbchild->child;
            arrchild = rbchild->child->rsibling;
            while(varchild->nodeNum == NODE_PAIR) varchild = varchild->child;
            switch(varchild->dataType.type){
            case TYPEINT:
                fprintf(fptr, "GET_ELEM_I");
                break;
            case TYPEFLOAT:
                fprintf(fptr, "GET_ELEM_F");
                break;
            case TYPESEQ:
                //TODO
                fprintf(fptr, "GET_ELEM_SEQ");
                switch(varchild->typeNode->child->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "_I");
                    break;
                case TYPEFLOAT :
                    fprintf(fptr, "_F");
                    break;
                default:
                    assert(0); //TODO
                }
                break;
            case TYPETUPLE:
                //TODO
                fprintf(fptr, "GET_ELEM_");
                gentypes(fptr, varchild);
                break;
            default:
                assert(0);
                break;
            }
            assert(varchild->string);
            fprintf(fptr, "(%s,%s,_i);\n",varchild->string,arrchild->string);
            if(varchild->nodeNum == RB_TUPLE){
                sqcodegen(fptr,varchild);
            }
            rbchild= rbchild->rsibling;
        }while(rbchild!=RBINDS->child);

        switch(varchild->dataType.type){
        case TYPEINT:
            fprintf(fptr, "SET_ELEM_I");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "SET_ELEM_F");
            break;
        case TYPESEQ:{
            int x=0;
            struct nodeType *loopme = varchild->typeNode->child;
            fprintf(fptr, "SET_ELEM_SEQ");
            while(loopme->dataType.type == TYPESEQ){
                fprintf(fptr,"_SEQ");
                loopme = loopme->typeNode->child;
                if(x++==10) abort();
            }
            switch(loopme->dataType.type){
            case TYPEINT:
                fprintf(fptr, "_I");
                break;
            case TYPEFLOAT:
                fprintf(fptr, "_F");
                break;
            case TYPETUPLE:
                fprintf(fptr, "_");
                gentypes(fptr, loopme->typeNode);
                break;
            }
            break;}
        case TYPETUPLE:
            fprintf(fptr, "SET_ELEM_");
            gentypes(fptr, varchild->typeNode);
            //TODO
            //assert(0);
            break;
        default:
            assert(0);
            break;
        }
        fprintf(fptr, "(%s,%s,_i);\n", varchild->string, node->string);
        forlooprefaddcount = refTable.size - forlooprefaddcount;
#ifdef DEBUG
        fprintf(fptr,"//DEBUG forlooprefaddcount:%d\n",forlooprefaddcount);
#endif
        ////DECREF(fptr,forlooprefaddcount);
        if(containArray(varchild)) {
            fprintf(fptr, "\n//app1 release(%s); %d\n", varchild->string, varchild->nodeNum);
            printDECREF(fptr,varchild);
        }
        fprintf(fptr, "}\n");// close for
        /* release the arrchilds */
        do{
            arrchild = rbchild->child->rsibling;
            if(containArray(arrchild)){
                //sqcodegen(fptr, arrchild);
                if(arrchild->nodeNum!=NODE_TOKEN){
                    fprintf(fptr, "\n//release arrchild :%s , nodeType:%d\n", arrchild->string, arrchild->nodeNum);
                    printDECREF(fptr, arrchild);
                }
            }
            else assert(0);
            rbchild = rbchild->rsibling;
        }while(rbchild!=RBINDS->child);



#if DEBUG
        fprintf(fptr,"//DEBUG refaddcount:%d\n",refaddcount);
#endif
        //DECREF(fptr,refaddcount);
        //node->typeNode = node->child;
        //printAddREF(fptr, node->string, node->dataType.type, node);
        fprintf(fptr, "}\n");//close scope

        break;
    }

    case NODE_ACTION_TUPLE:{
        // FIXME support no nested situation.
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = node->child->rsibling;
        struct nodeType *tmp = LHS;
        do{
            tmp = LHS;
            while(LHS->nodeNum ==NODE_PAIR) LHS=LHS->child;
            switch(LHS->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, LHS);
                break;
            }
            LHS=tmp;
            LHS= LHS->rsibling;
        }while(LHS!=node->child);

        fprintf(fptr, "%s.a = ",node->string);
        while(LHS->nodeNum ==NODE_PAIR) LHS=LHS->child;
        switch(LHS->nodeNum){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
            sqcodegen(fptr, LHS);
            break;
        default:
            fprintf(fptr,"%s", LHS->string);
            break;
        }
        LHS=tmp;
        LHS= LHS->rsibling;
        fprintf(fptr, ";\n");

        fprintf(fptr, "%s.b = ", node->string);
        while(RHS->nodeNum ==NODE_PAIR) RHS=RHS->child;
        switch(RHS->nodeNum){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
            sqcodegen(fptr, RHS);
            break;
        default:
            fprintf(fptr, "%s", RHS->string);
            break;
        }
        fprintf(fptr, ";\n");
        RHS=tmp;
        RHS= RHS->rsibling;

        //assert(0); //not implement;
        break;
    }
    case NODE_APPLYBODY2:{
        struct nodeType *LHS = node->child;
        struct nodeType *RBINDS = LHS->rsibling;
        struct nodeType *rbchild = RBINDS->child;
        struct nodeType *varchild = rbchild->child;
        struct nodeType *arrchild = varchild->rsibling;
        int refaddcount=0;
        int forlooprefaddcount=0;

        //open scope
        fprintf(fptr, "{\n");
        dumpTable(fptr, node);

        /*generate the src arrays.*/
        refaddcount = refTable.size;
        //sqcodegen(fptr, SRCARR);
        do{
            arrchild = rbchild->child->rsibling;
            if(arrchild->nodeNum!=NODE_TOKEN)
                sqcodegen(fptr, arrchild);
            rbchild = rbchild->rsibling;
        }while(rbchild!=RBINDS->child);

        refaddcount = refTable.size-refaddcount; 

        /* allocate the dest array.*/
        fprintf(fptr, "MALLOC(%s,%s.len,",node->string, arrchild->string);

        switch(LHS->dataType.type){
        case TYPEINT:
            fprintf(fptr, "int");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "float");
            break;
        case TYPEBOOL:
            fprintf(fptr, "bool");
            break;
        case TYPECHAR:
            fprintf(fptr, "char");
            break;
        case TYPESEQ:
            fprintf(fptr, "struct Sequence");
            break;
        case TYPETUPLE:
            fprintf(fptr, "struct ");
            gentypes(fptr,LHS);
            break;
        default:
            assert(0);
            break;
        }
        fprintf(fptr,");\n");

        /* generate for loop */
        forlooprefaddcount = refTable.size;
        fprintf(fptr, "_len = %s.len;\n",node->string);
        fprintf(fptr, "#pragma pf parallel_rr\n");
        fprintf(fptr, "for (_i =0; _i <_len;_i++){\n");
        dumpTable(fptr, node->child); 
        /* get elem from src array.*/
        do{
            varchild = rbchild->child;
            arrchild = rbchild->child->rsibling;
            while(varchild->nodeNum == NODE_PAIR) varchild = varchild->child;
            switch(varchild->dataType.type){
            case TYPEINT:
                fprintf(fptr, "GET_ELEM_I");
                break;
            case TYPEFLOAT:
                fprintf(fptr, "GET_ELEM_F");
                break;
            case TYPESEQ:
                //TODO
                fprintf(fptr, "GET_ELEM_SEQ");
                switch(varchild->typeNode->child->dataType.type){
                case TYPEINT:
                    fprintf(fptr, "_I");
                    break;
                case TYPEFLOAT :
                    fprintf(fptr, "_F");
                    break;
                default:
                    assert(0); //TODO
                }
                break;
            case TYPETUPLE:
                //TODO
                fprintf(fptr, "GET_ELEM_");
                gentypes(fptr, varchild);
                break;
            default:
                assert(0);
                break;
            }
            assert(varchild->string);
            fprintf(fptr, "(%s,%s,_i);\n",varchild->string,arrchild->string);
            if(varchild->nodeNum == RB_TUPLE){
                sqcodegen(fptr,varchild);
            }
            rbchild= rbchild->rsibling;
        }while(rbchild!=RBINDS->child);

        switch(LHS->nodeNum){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_BOOL:
        case NODE_CHAR:
        case NODE_TOKEN:
            break;
        default:
            sqcodegen(fptr, LHS);
        }

        switch(LHS->dataType.type){
        case TYPEINT:
            fprintf(fptr, "SET_ELEM_I");
            break;
        case TYPEFLOAT:
            fprintf(fptr, "SET_ELEM_F");
            break;
        case TYPESEQ:{
            int x=0;
            struct nodeType *loopme = LHS->typeNode->child;
            fprintf(fptr, "SET_ELEM_SEQ");
            while(loopme->dataType.type == TYPESEQ){
                fprintf(fptr,"_SEQ");
                loopme = loopme->typeNode->child;
                if(x++==10) abort();
            }
            switch(loopme->dataType.type){
            case TYPEINT:
                fprintf(fptr, "_I");
                break;
            case TYPEFLOAT:
                fprintf(fptr, "_F");
                break;
            case TYPETUPLE:
                fprintf(fptr, "_");
                gentypes(fptr, loopme->typeNode);
                break;
            }
            break;}
        case TYPETUPLE:
            fprintf(fptr, "SET_ELEM_");
            gentypes(fptr, LHS->typeNode);
            //TODO
            //assert(0);
            break;
        default:
            assert(0);
            break;
        }
        fprintf(fptr, "(%s,%s,_i);\n", LHS->string, node->string);
        forlooprefaddcount = refTable.size - forlooprefaddcount;
#ifdef DEBUG
        fprintf(fptr,"//DEBUG forlooprefaddcount:%d\n",forlooprefaddcount);
#endif
        ////DECREF(fptr,forlooprefaddcount);
        if(containArray(LHS)) {
            fprintf(fptr, "\n//release(%s); %d\n", LHS->string, LHS->nodeNum);
            printDECREF(fptr,LHS);
        }
        fprintf(fptr, "}\n");// close for
        /* release the arrchilds */
        do{
            arrchild = rbchild->child->rsibling;
            if(containArray(arrchild)){
                //sqcodegen(fptr, arrchild);
                if(arrchild->nodeNum!=NODE_TOKEN){
                    fprintf(fptr, "\n//release arrchild :%s , nodeType:%d\n", arrchild->string, arrchild->nodeNum);
                    printDECREF(fptr, arrchild);
                }
            }
            else assert(0);
            rbchild = rbchild->rsibling;
        }while(rbchild!=RBINDS->child);



#if DEBUG
        fprintf(fptr,"//DEBUG refaddcount:%d\n",refaddcount);
#endif
        //DECREF(fptr,refaddcount);
        //node->typeNode = node->child;
        //printAddREF(fptr, node->string, node->dataType.type, node);
        fprintf(fptr, "}\n");//close scope

        break;
    }
    case GEN_APP3:{
        struct nodeType *APP3 = node->child->rsibling;
        struct nodeType *RBINDS = APP3->child;
        struct nodeType *rbchild = RBINDS->child;
        struct nodeType *FREVAR = rbchild->child;
        struct nodeType *SRCARR = FREVAR->rsibling;
        struct nodeType *FILTER = RBINDS->rsibling; 

        //do{}while(rbchild!=RBINDS->child); //this is for future FILTER_2.
        //if(rbchild->rsibling != rbchild){}
        fprintf(fptr, "FILTER_%d(%s, %s, ",RBINDS->counts,node->string, FREVAR->string);

        switch(node->dataType.type){
        case TYPESEQ:
            switch(node->typeNode->child->dataType.type){
            case TYPETUPLE:
                printtype(fptr, node->typeNode->child->typeNode);
                fprintf(fptr, ", ");
                gentypes(fptr, node->typeNode->child->typeNode);
                fprintf(fptr, ",\n");
                break; 
            case TYPEINT:
                fprintf(fptr, " int, I,\n");
                break;
            case TYPEFLOAT:
                fprintf(fptr, " float, F,\n");
                break;
            case TYPEBOOL:
            case TYPECHAR:
            default:
                assert(0);//not implement;
            }
            break;
        default: 
            assert(0); // not implement;
        }

        do{
            while(FREVAR->nodeNum == NODE_PAIR) FREVAR = FREVAR->child;
            fprintf(fptr, "%s, %s, ", SRCARR->string, FREVAR->string);
            switch(FREVAR->dataType.type){
            case TYPESEQ:
                switch(FREVAR->typeNode->dataType.type){
                case TYPETUPLE:
                    assert(0);
                    break; 
                default:
                    assert(0);//not implement;
                }
                break;
            case TYPEINT:
                fprintf(fptr, " int, I,\n");
                break;
            case TYPEFLOAT:
                fprintf(fptr, " float, F,\n");
                break;
            case TYPETUPLE:
                printtype(fptr, node->typeNode->child->typeNode);
                fprintf(fptr, ", ");
                gentypes(fptr, node->typeNode->child->typeNode);
                fprintf(fptr, ",\n");
                break;
            default: 
                assert(0); // not implement;
            }
            rbchild = rbchild->rsibling;
            FREVAR = rbchild->child;
            SRCARR = FREVAR->rsibling;
        }while(rbchild!=RBINDS->child);

        fprintf(fptr, "(");
        sqcodegen(fptr,FILTER->child);
        fprintf(fptr, "));\n");

        //node->typeNode = FREVAR->typeNode;
        //assert(node->typeNode); 
        //printAddREF(fptr, node->string, node->dataType.type, node);

        break;
    } // end of GEN_APP3

    case NODE_APPLYBODY3:{
        struct nodeType *RBINDS = node->child;
        struct nodeType *rbchild = RBINDS->child;
        struct nodeType *FREVAR = rbchild->child;
        struct nodeType *SRCARR = FREVAR->rsibling;
        struct nodeType *FILTER = RBINDS->rsibling; // TODO remove the pair in yacc.
        fprintf(fptr, "{\n");
        dumpTable(fptr, node);
        switch(SRCARR->nodeNum){
        case NODE_TOKEN:
            break;
        default:
            sqcodegen(fptr, SRCARR);
            break;
        }
        switch(FREVAR->nodeNum){
        case RB_TUPLE:
            //sqcodegen(fptr,FREVAR);
            fprintf(fptr, "FILTER_TUPLE_%d(%s, %s,", 
                    RBINDS->counts,node->string, FREVAR->string);
            // FIXME dirtyway
            fprintf(fptr, "%s, %s,", FREVAR->child->string, FREVAR->child->rsibling->string);
            printtype(fptr, FREVAR->child);
            fprintf(fptr, ", ");
            printtype(fptr, FREVAR->child->rsibling);
            fprintf(fptr, ",\n");
            break;
        default:
            fprintf(fptr, "FILTER_%d(%s, %s, ",RBINDS->counts,node->string, FREVAR->string);
            break;
        }

        switch(node->dataType.type){
        case TYPESEQ:
            switch(node->typeNode->child->dataType.type){
            case TYPETUPLE:
                printtype(fptr, node->typeNode->child);
                fprintf(fptr, ", ");
                gentypes(fptr, node->typeNode->child);
                fprintf(fptr, ",\n ");
                break; 
            case TYPEFLOAT:
                fprintf(fptr, "float , F,\n");
                break;
            case TYPEINT:
                fprintf(fptr, "int , I,\n");
                break;
            case TYPEBOOL:
                fprintf(fptr, "bool , B,\n");
                break;
            case TYPECHAR:
                fprintf(fptr, "char , C,\n");
                break;
            default:
                assert(0);//not implement;
            }
            break;
        default: 
            assert(0); // not implement;
        }

        do{
            while(FREVAR->nodeNum == NODE_PAIR) FREVAR = FREVAR->child;
            fprintf(fptr, "%s, %s, ", SRCARR->string, FREVAR->string);
            switch(FREVAR->dataType.type){
            case TYPESEQ:
                switch(FREVAR->typeNode->child->dataType.type){
                case TYPETUPLE:
                    assert(0);
                    break; 
                default:
                    assert(0);//not implement;
                }
                break;
            case TYPEINT:
                fprintf(fptr, " int, I,\n");
                break;
            case TYPEFLOAT:
                fprintf(fptr, " float, F,\n");
                break;
            case TYPETUPLE:
                printtype(fptr, FREVAR->typeNode);
                fprintf(fptr, ", ");
                gentypes(fptr, FREVAR->typeNode);
                fprintf(fptr, ",\n");
                //assert(0);
                break;
            default: 
                assert(0); // not implement;
            }
            rbchild = rbchild->rsibling;
            FREVAR = rbchild->child;
            SRCARR = FREVAR->rsibling;
        }while(rbchild!=RBINDS->child);

        fprintf(fptr, "(");
        sqcodegen(fptr,FILTER->child);
        fprintf(fptr, "));\n");
        fprintf(fptr, "}\n"); 

        //node->typeNode = FREVAR;
        assert(node->typeNode); 

        //FIXME dirty4ni~~
        if(node->parent->nodeNum == NODE_LETRET){
            printf("boom\n");
        }else{
            //printAddREF(fptr, node->string, node->dataType.type, node);
        }

        break;
    }

    case NODE_APPLYBODY4:{
        // body+rbind+filter
        struct nodeType *LHS = node->child;
        struct nodeType *RBINDS = LHS->rsibling;
        struct nodeType *rbchild = RBINDS->child;
        struct nodeType *FREVAR = rbchild->child;
        struct nodeType *SRCARR = FREVAR->rsibling;
        struct nodeType *FILTER = RBINDS->rsibling; // TODO remove the pair in yacc.
        fprintf(fptr, "{\n");
        dumpTable(fptr, node);
        switch(SRCARR->nodeNum){
        case NODE_TOKEN:
            break;
        default:
            sqcodegen(fptr, SRCARR);
            break;
        }
        switch(FREVAR->nodeNum){
        case RB_TUPLE:
            assert(0); // TODO complex body should be transformed as a function.
            //sqcodegen(fptr,FREVAR);
            fprintf(fptr, "FILTER_TUPLE_%d(%s, %s,", 
                    RBINDS->counts,node->string, FREVAR->string);
            // FIXME dirtyway
            fprintf(fptr, "%s, %s,", FREVAR->child->string, FREVAR->child->rsibling->string);
            printtype(fptr, FREVAR->child);
            fprintf(fptr, ", ");
            printtype(fptr, FREVAR->child->rsibling);
            fprintf(fptr, ",\n");
            break;
        default:
            // generate FILTER
            fprintf(fptr, "FILTER_%d(%s, ",RBINDS->counts,node->string );

            // generate body 
            switch(LHS->nodeNum){
            case NODE_FUNC_CALL:
                //TODO only simple FUNCTION call can be used.
                assert(0);
                break;
            default:
                sqcodegen(fptr, LHS);
                fprintf(fptr, ", ");
                break;
            }

            break;
        }

        switch(node->dataType.type){
        case TYPESEQ:
            switch(node->typeNode->child->dataType.type){
            case TYPETUPLE:
                printtype(fptr, node->typeNode->child);
                fprintf(fptr, ", ");
                gentypes(fptr, node->typeNode->child);
                fprintf(fptr, ",\n ");
                break; 
            case TYPEFLOAT:
                fprintf(fptr, "float , F,\n");
                break;
            case TYPEINT:
                fprintf(fptr, "int , I,\n");
                break;
            case TYPEBOOL:
                fprintf(fptr, "bool , B,\n");
                break;
            case TYPECHAR:
                fprintf(fptr, "char , C,\n");
                break;
            default:
                assert(0);//not implement;
            }
            break;
        default: 
            assert(0); // not implement;
        }

        do{
            while(FREVAR->nodeNum == NODE_PAIR) FREVAR = FREVAR->child;
            fprintf(fptr, "%s, %s, ", SRCARR->string, FREVAR->string);
            switch(FREVAR->dataType.type){
            case TYPESEQ:
                switch(FREVAR->typeNode->child->dataType.type){
                case TYPETUPLE:
                    assert(0);
                    break; 
                default:
                    assert(0);//not implement;
                }
                break;
            case TYPEINT:
                fprintf(fptr, " int, I,\n");
                break;
            case TYPEFLOAT:
                fprintf(fptr, " float, F,\n");
                break;
            case TYPETUPLE:
                printtype(fptr, FREVAR->typeNode);
                fprintf(fptr, ", ");
                gentypes(fptr, FREVAR->typeNode);
                fprintf(fptr, ",\n");
                //assert(0);
                break;
            default: 
                assert(0); // not implement;
            }
            rbchild = rbchild->rsibling;
            FREVAR = rbchild->child;
            SRCARR = FREVAR->rsibling;
        }while(rbchild!=RBINDS->child);

        fprintf(fptr, "(");
        sqcodegen(fptr,FILTER->child);
        fprintf(fptr, "));\n");
        fprintf(fptr, "}\n"); 

        //node->typeNode = FREVAR;
        assert(node->typeNode); 

        //FIXME dirty4ni~~
        if(node->parent->nodeNum == NODE_LETRET){
            printf("boom\n");
        }else{
            //printAddREF(fptr, node->string, node->dataType.type, node);
        }

        break;
    }
    case NODE_RBINDS:
        //FIXME might have many children
        // has been done by the upper level apply-to-each.
        sqcodegen(fptr, node->child);
        break;

    case NODE_IN:
        //FIXME here only consider the simple ApplyBody Exp.
        //assert(node->child->nodeNum == NODE_TOKEN);
        //assert(node->child->rsibling->nodeNum == NODE_TOKEN);

        sqcodegen(fptr, node->child->rsibling);
        //    node->string
        break;

    case RB_TUPLE:
    case LHS_TUPLE:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = LHS->rsibling;

        assert(node->string);  

        switch (LHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            sqcodegen(fptr, LHS);
            fprintf(fptr, " = ");
            break;
        default:
            while(LHS->nodeNum == NODE_PAIR) LHS = LHS->child;
            fprintf(fptr,"%s = ",LHS->string);
            break;
        }
        fprintf(fptr,"%s.a;\n",node->string);

        switch (RHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            sqcodegen(fptr, RHS);
            fprintf(fptr, " = ");
            break;
        default:
            while(RHS->nodeNum == NODE_PAIR) RHS = RHS->child;
            fprintf(fptr,"%s = ",RHS->string);
            break;
        }
        fprintf(fptr,"%s.b;\n",node->string);

        switch (LHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            break;
        default:
            while(LHS->nodeNum == NODE_PAIR) LHS = LHS->child;
            sqcodegen(fptr, LHS);
            LHS= node->child;
            break;
        }
        switch (RHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            break;
        default:
            while(RHS->nodeNum == NODE_PAIR) RHS = RHS->child;
            sqcodegen(fptr, RHS);
            assert(RHS->string);
            RHS= LHS->rsibling;
            break;
        }


        break;
    }
    case ELEM_TUPLE:
    case RHS_TUPLE:
    case NODE_TUPLE:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = LHS->rsibling;
        switch (LHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            break;
        default:
            while(LHS->nodeNum == NODE_PAIR) LHS = LHS->child;
            sqcodegen(fptr, LHS);
            LHS= node->child;
            break;
        }
        switch (RHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            break;
        default:
            while(RHS->nodeNum == NODE_PAIR) RHS = RHS->child;
            sqcodegen(fptr, RHS);
            assert(RHS->string);
            RHS= LHS->rsibling;
            break;
        }
        assert(node->string);
        fprintf(fptr,"%s.a = ",node->string);
        switch (LHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            sqcodegen(fptr, LHS);
            break;
        default:
            while(LHS->nodeNum == NODE_PAIR) LHS = LHS->child;
            fprintf(fptr,"%s",LHS->string);
            break;
        }

        fprintf(fptr,";\n%s.b = ",node->string);
        switch (RHS->nodeNum ){
        case NODE_INT:
        case NODE_FLOAT:
        case NODE_CHAR:
        case NODE_BOOL:
        case NODE_TOKEN:
            sqcodegen(fptr, RHS);
            break;
        case NODE_PAIR:
            while(!RHS->string) RHS=RHS->child;
            fprintf(fptr,"%s",RHS->string);
            RHS = LHS->rsibling;
            break;
        default:
            fprintf(fptr,"%s",RHS->string);
            break;
        }
        fprintf(fptr, ";\n"); 
        break;
    } // end of NODE_TUPLE

    case NODE_PAIR:
        if(node->child->nodeNum==NODE_TUPLE){
            if(node->isEndofFunction)
                node->child->isEndofFunction = node->isEndofFunction;
            sqcodegen(fptr, node->child);
        }else{
            fprintf(fptr, "(");
            assert(child);
            //sqcodegen(fptr,node->child);
            do{
                sqcodegen(fptr,node->child);
                child = child->rsibling;
            }while(child!=node->child);
            fprintf(fptr, ")");
        }
        break;
    case NODE_INT:
        fprintf(fptr," %d ",node->iValue);
        break;
    case NODE_FLOAT:
        fprintf(fptr," %f ",node->rValue);
        break;

    case NODE_FUNC_CALL:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = LHS->rsibling;

        if(strcmp(node->child->string, "isContiguousList")==0){
            struct nodeType *param1,*param2,*param3,*tuple1;
            while(RHS->nodeNum==NODE_PAIR) RHS=RHS->child;
            assert(RHS->nodeNum == PARAM_TUPLE);
            param1 = RHS->child;
            assert(param1->rsibling->nodeNum == PARAM_TUPLE);
            tuple1 =param1->rsibling;
            param2 = tuple1->child;
            param3 = param2->rsibling;
            for(int i =0;i<3;i++){
                if (i==0) param1 = param1;
                if (i==1) param1 = param2;
                if (i==2) param1 = param3;
                switch(param1->nodeNum){
                case NODE_INT:
                case NODE_TOKEN:
                    break;
                default:
                    sqcodegen(fptr,param1);
                }
            } 
            param1 = RHS->child;
            fprintf(fptr, "%s = isContiguousList(",node->string);
            for(int i =0;i<3;i++){
                if (i==0) param1 = param1;
                if (i==1) param1 = param2;
                if (i==2) param1 = param3;
                switch(param1->nodeNum){
                case NODE_INT:
                case NODE_TOKEN:
                    sqcodegen(fptr,param1);
                    break;
                default:
                    fprintf(fptr,"%s",param1->string);
                }
                if(i<2) fprintf(fptr,",");
            }
            fprintf(fptr,");\n");

        }else if(strcmp(node->child->string, "genShuffledList")==0){
            struct nodeType *param1,*param2,*param3,*tuple1;

            while(RHS->nodeNum==NODE_PAIR) RHS=RHS->child;
            assert(RHS->nodeNum == PARAM_TUPLE);
            param1 = RHS->child;
            param2 = RHS->child->rsibling;

            for(int i =0;i<2;i++){
                if (i==0) param1 = param1;
                if (i==1) param1 = param2;
                if (i==2) param1 = param3;
                switch(param1->nodeNum){
                case NODE_INT:
                case NODE_TOKEN:
                    break;
                default:
                    sqcodegen(fptr,param1);
                }
            } 
            param1 = RHS->child;
            fprintf(fptr, "%s = genShuffledList(",node->string);
            for(int i =0;i<2;i++){
                if (i==0) param1 = param1;
                if (i==1) param1 = param2;
                if (i==2) param1 = param3;
                switch(param1->nodeNum){
                case NODE_INT:
                case NODE_TOKEN:
                    sqcodegen(fptr,param1);
                    break;
                default:
                    fprintf(fptr,"%s",param1->string);
                }
                if(i<1) fprintf(fptr,",");
            }
            fprintf(fptr,");\n");
        }else if(strcmp(node->child->string, "verifyQHull")==0){
            while(RHS->nodeNum == NODE_PAIR) RHS=RHS->child;
            struct nodeType *param1 = RHS->child;
            struct nodeType *param2 = param1->rsibling;
            assert(param1->dataType.type ==TYPEINT);
            assert(param2->dataType.type ==TYPESEQ);
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_TOKEN:
                break;
            default: 
                sqcodegen(fptr, param1);
                break;
            }
            switch(param2->nodeNum){
            case NODE_INT:
            case NODE_TOKEN:
                break;
            default: 
                sqcodegen(fptr, param2);
                break;
            }
            fprintf(fptr, "%s = verifyQHull(",node->string);
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_TOKEN:
                sqcodegen(fptr, param1);
                break;
            default: 
                fprintf(fptr, "%s",param1->string);
                break;
            }
            fprintf(fptr, ",");
            switch(param2->nodeNum){
            case NODE_INT:
            case NODE_TOKEN:
                sqcodegen(fptr, param2);
                break;
            default: 
                fprintf(fptr, "%s",param2->string);
                break;
            }
            fprintf(fptr, ");\n");
        }else if(strcmp(node->child->string, "genReverseList")==0){
            struct nodeType* param1 = RHS;
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                break;
            default:
                while(param1->nodeNum == NODE_PAIR) param1=param1->child;
                sqcodegen(fptr,param1);
                param1 = RHS->child->child;
            }
            fprintf(fptr,"%s = genReverseList(", node->string);
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                sqcodegen(fptr,param1);
                break;
            default:
                while(param1->nodeNum == NODE_PAIR) param1=param1->child;
                fprintf(fptr,"%s", param1->string);
                param1 = RHS->child->child;
            }
            fprintf(fptr, ");\n");

        }else if(strcmp(node->child->string, "dist")==0){
            struct nodeType* param1 = RHS->child;
            struct nodeType* param2 = RHS->child->rsibling;

            //first time it will prepare the parameters
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                break;
            default:
                while(param1->nodeNum == NODE_PAIR) param1=param1->child;
                sqcodegen(fptr,param1);
                param1 = param2->rsibling;
            }
            switch(param2->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                break;
            default:
                while(param2->nodeNum == NODE_PAIR) param2=param2->child;
                sqcodegen(fptr,param2);
                param2 = param1->rsibling;
            }
            if(param1->dataType.type == TYPEINT)
                fprintf(fptr, "NESLDIST(%s,",node->string);
            else if(param1->dataType.type == TYPEFLOAT)
                fprintf(fptr, "NESLDIST_F(%s,",node->string);
            else assert(0); // not support.
            switch(param1->nodeNum){
            case NODE_TOKEN:
                fprintf(fptr,"%s,",param1->string);
                break;
            case NODE_INT:
                fprintf(fptr,"%d,",param1->iValue);
                break;
            case NODE_FLOAT:
                fprintf(fptr, "%f,",param1->rValue);
                break;
            default:
                assert(0);
            }
            switch(param2->nodeNum){
            case NODE_TOKEN:
                fprintf(fptr,"%s);\n",param2->string);
                break;
            case NODE_INT:
                fprintf(fptr,"%d);\n",param2->iValue);
                break;
            default:
                assert(0);
            }
        }else if(strcmp(node->child->string, "time")==0){
            fprintf(fptr, "{\nDECT1T2\nfloat diff;\n");
            //printtype(fptr, RHS->dataType.type);
            //fprintf(fptr, " %s;\n", RHS->child->string);
            fprintf(fptr, "_t1 = CLOCK();\n");
            sqcodegen(fptr, RHS);
            //FIXME might be single number or a function name without parameter.
            // too many possibilies.
            fprintf(fptr, "_t2 = CLOCK();\n");

            fprintf(fptr, "diff = ((float)(_t2 - _t1) / CLOCKSPEED);\n");
            //fprintf(fptr, "tm = diff;\n");
            int count =0;
            while(!RHS->string && count<10) {RHS=RHS->child; count++;}
            //FIXME dirty
            //if(RHS->dataType.type >=TYPESEQ_I&&RHS->dataType.type<=TYPESEQ)
            //  RHS->isEndofFunction = 1; 
#ifdef DEBUG
            fprintf(fptr, "//DEBUG %s.isEndofFunction:%d\n",node->string, node->isEndofFunction);
            fprintf(fptr, "//DEBUG %s.isEndofFunction:%d\n",RHS->string, RHS->isEndofFunction);
#endif
            fprintf(fptr, "%s.a = %s;\n", node->string, RHS->string);
            RHS=LHS->rsibling;
            fprintf(fptr, "%s.b = diff;\n", node->string);
            fprintf(fptr, "}//end of function time(...)\n");

        }else if(strcmp(node->child->string, "rand")==0){
            struct nodeType *RHS = node->child->rsibling;
            while(RHS->nodeNum == NODE_PAIR) RHS=RHS->child;
            switch(RHS->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr,RHS);
            }
            RHS = node->child->rsibling;

            fprintf(fptr,"%s = ",node->string);
            if(node->dataType.type ==TYPEINT)
                fprintf(fptr, "RAND_I("); 
            else 
                fprintf(fptr, "RAND_F(");

            while(RHS->nodeNum == NODE_PAIR) RHS=RHS->child;
            switch(RHS->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                sqcodegen(fptr, RHS);
                break;
            default:{
                int count =0;
                while(!RHS->string && count<10) {RHS=RHS->child; count++;}
                fprintf(fptr,"%s",RHS->string);
                RHS= node->child->rsibling;
                break;
            }
            }
            fprintf(fptr,");\n");

        }else if(strcmp(node->child->string, "plusp") ==0){
            struct nodeType *param = node->child->rsibling;
            while(param->nodeNum == NODE_PAIR) param = param->child;
            assert(!param->isvisited);
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param);
                break;
            }
            fprintf(fptr, "plusp(");
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param);
                break;
            default:
                fprintf(fptr, "%s", param->string);
                break;
            }
            fprintf(fptr, ")");
            if(node->parent->nodeNum!=NODE_FILTER) fprintf(fptr,";\n");
        }else if(strcmp(node->child->string, "flatten") == 0){
            struct nodeType *param = node->child->rsibling;
            struct nodeType *typer = param->child->typeNode;
            while(param->nodeNum==NODE_PAIR) param = param->child;
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param);
                break;
            }

            fprintf(fptr, "//add refcnt inside header file\n");
            fprintf(fptr, "FLATTEN(");

            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param);
                break;
            default:
                fprintf(fptr, "%s",param->string);
                break;
            }
            fprintf(fptr, ",%s,",node->string);

            switch(typer->child->dataType.type){
            case TYPETUPLE:
                printtype(fptr, typer->child->typeNode);
                fprintf(fptr, ",");
                gentypes(fptr, typer->child->typeNode);
                break;
            default:
                assert(0);
                break;
            }
            fprintf(fptr, ");\n");
            break;
            //assert(0);
        }else if(strcmp(LHS->string, "max_index") == 0){
            struct nodeType *param = node->child->rsibling;
            while(param->nodeNum==NODE_PAIR) param = param->child;
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param);
                break;
            }
            switch (param->dataType.type){
            case TYPESEQ: 
                fprintf(fptr, "%s = max_index_f(",node->string);
                break;
            default:
                assert(0);// not implement;
            }
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param);
                break;
            default:
                fprintf(fptr, "%s",param->string);
                break;
            }
            fprintf(fptr, ");\n");
            return ;
        }else if(strcmp(LHS->string, "min_index") == 0){
            struct nodeType *param = node->child->rsibling;
            while(param->nodeNum==NODE_PAIR) param = param->child;
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param);
                break;
            }
            switch (param->dataType.type){
            case TYPESEQ: 
                fprintf(fptr, "%s = min_index_f(",node->string);
                break;
            default:
                assert(0);// not implement;
            }
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param);
                break;
            default:
                fprintf(fptr, "%s",param->string);
                break;
            }
            fprintf(fptr, ");\n");
            return ;
        }else if(strcmp(LHS->string, "float") == 0){
            struct nodeType *param = node->child->rsibling;
            while(param->nodeNum==NODE_PAIR) param = param->child;
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param);
                break;
            }
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                fprintf(fptr, "%s = (float)",node->string);
                sqcodegen(fptr, param);
                fprintf(fptr, ";\n");
                break;
            default:
                fprintf(fptr, "%s = (float)%s;\n",node->string,param->string);
                break;
            } 
            return;
        }else if(strcmp(LHS->string, "round") == 0){
            struct nodeType *param = node->child->rsibling;
            while(param->nodeNum==NODE_PAIR) param = param->child;
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param);
                break;
            }
            fprintf(fptr, "%s = nearbyint(",node->string);
            switch(param->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param);
                break;
            default:
                fprintf(fptr, "%s",param->string);
                break;
            }    
            fprintf(fptr, ");\n");
        }else if(strcmp(LHS->string, "print_string") == 0){
            struct nodeType *param = node->child->rsibling;
            while(param->nodeNum==NODE_PAIR) param = param->child;
            assert(param ->nodeNum == NODE_STRING);

            fprintf(fptr, "printf(%s);\n", param->string);
            fprintf(fptr, "%s = 1;\n",node->string);
            return ;
        }else if(strcmp(LHS->string, "mod") == 0){
            struct nodeType *param1 = node->child->rsibling->child->child;
            struct nodeType *param2 = param1->rsibling;
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param1);
            }
            switch(param2->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                break;
            default:
                sqcodegen(fptr, param2);
            }
            fprintf(fptr, "%s = ",node->string);
            switch(param1->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param1);
                break;
            default:
                fprintf(fptr, "%s",param1->string);
            }
            /* print '%' in ascii */
            fprintf(fptr, "%c",37);
            switch(param2->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_BOOL:
            case NODE_CHAR:
            case NODE_TOKEN:
                sqcodegen(fptr, param2);
                break;
            default:
                fprintf(fptr, "%s",param2->string);
            }
            fprintf(fptr, ";\n");
            break;
        }else if(strcmp(LHS->string, "sum") == 0){
            assert(0);
        }else {
            // user define function.
            while(RHS->nodeNum == NODE_PAIR) RHS= RHS->child;
            //first time it will prepare the parameters
            switch(RHS->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                break;
            default:
                while(RHS->nodeNum == NODE_PAIR) RHS=RHS->child;
                sqcodegen(fptr,RHS);
                RHS = LHS->rsibling;
            }
            fprintf(fptr, "%s = %s",node->string, LHS->string);
            fprintf(fptr,"(");

            switch(RHS->nodeNum){
            case NODE_INT:
            case NODE_FLOAT:
            case NODE_CHAR:
            case NODE_BOOL:
            case NODE_TOKEN:
                sqcodegen(fptr,RHS);
                break;
            default:
                while(RHS->nodeNum == NODE_PAIR) RHS = RHS->child;
                if(RHS->nodeNum == PARAM_TUPLE)
                    //sqcodegen(fptr, RHS); //second time it will print all the parameter names.
                    fprintf(fptr, "%s", RHS->string);
                else
                    fprintf(fptr, "%s", RHS->string);
                RHS = LHS->rsibling;
            }
            fprintf(fptr, ");\n");
        }

        if(node->dataType.type==TYPESEQ)
            //printAddREF(fptr,node->string, node->dataType.type, node);
            ;
        break;
    }// end of NODE_FUNC_CALL
    case NODE_TOKEN:

        if(node->parent->nodeNum != NODE_NESL){
            switch(node->token){
            case TOKE_ID:{
                struct SymTableEntry* entry = findSymbol(node->table, node->string, REFERENCE);
                assert(entry);
                fprintf(fptr, "%s", node->string);
                break;
            }
            default:
                assert(0);
                break;
            }
        }
        break;
    default:
        break;
    }// switch node->nodeNum
    
    /**
    * print the result value
    */
    if(node->parent){
        if(node->parent->nodeNum == NODE_NESL 
           &&node->nodeNum!=NODE_FUNC
           &&node->nodeNum!=NODE_ASSIGN
           //      &&node->nodeNum!=NODE_APPLYBODY2
           //      &&node->nodeNum!=NODE_APPLYBODY3
          ){
            switch(node->dataType.type){
                struct nodeType *loopme; int x;
            case TYPEINT:
                if(node->nodeNum==NODE_INT) fprintf(fptr,"printf(%d\\n);\n",node->iValue);
                else fprintf(fptr, "print_I(%s);\n",node->string);
                break;
            case TYPECHAR:
                assert(0);
                break;
            case TYPEFLOAT:
                if(node->nodeNum==NODE_INT) fprintf(fptr,"printf(%f\\n);\n",node->rValue);
                else fprintf(fptr, "print_F(%s);\n",node->string);
                break;
            case TYPEBOOL:
                assert(0);
                break;
            case TYPETUPLE:{
                struct nodeType *child = node->typeNode->child;
                fprintf(fptr, "print_Tuple(%s,", node->string);
                do{
                    switch(child->dataType.type){
                    case TYPESEQ:
                        gentypes(fptr, child);
                        //assert(0); // TODO use the gentype(); 
                        break;
                    case TYPEFLOAT:
                        fprintf(fptr, "F");
                        break;
                    case TYPEINT:
                        fprintf(fptr, "I");
                        break;
                    default:
                        assert(0);
                    }
                    if(child==node->typeNode->child)
                        fprintf(fptr,",");
                    child = child->rsibling;
                }while(child!=node->typeNode->child);
                fprintf(fptr,");\n");
                break;
            }
            case TYPESEQ:{
                fprintf(fptr, "print_SEQ_");
                loopme = node->typeNode->child; 
                x=0;
                while(loopme->dataType.type ==TYPESEQ){
                    fprintf(fptr, "SEQ_");
                    loopme = loopme->typeNode->child;
                    assert(loopme);
                    if(x++==10) abort();//error;
                }
                switch(loopme->dataType.type){
                case TYPESEQ:  
                    fprintf(fptr, "SEQ_");
                    //assert(0);//not implement;
                    break;
                case TYPEINT:
                    fprintf(fptr, "I");
                    break;
                case TYPEFLOAT:
                    fprintf(fptr, "F");
                    break;
                case TYPETUPLE:
                    gentypes(fptr, loopme);
                    //fprintf(fptr, "T_");
                    break;
                default:
                    assert(0); //not implement
                }
                fprintf(fptr, "(%s);\n", node->string);
                break;
            }
            default:
                assert(0);
                break;
            }
        }
    }

    /*
     * print addrefcnt 
     */
    if(containArray(node)){
        switch(node->nodeNum){
        case NODE_APPLYBODY1:
        case NODE_APPLYBODY2:
        case NODE_APPLYBODY3:
        case NODE_APPLYBODY4:
        case GEN_APP3:
        case NODE_NEW_SEQ:
            //case NODE_LET:
        case PARAM_TUPLE:
            fprintf(fptr, "//containArray(node):%d\n",node->nodeNum);
            printAddREF(fptr,node->string,node->dataType.type,node); 
            break;
        case NODE_OP:
            switch(node->op){
            case OP_PP:
                fprintf(fptr, "//containArray(node):%d\n",node->nodeNum);
                printAddREF(fptr,node->string,node->dataType.type,node); 
                break;
            default:
                break; 
            }
            break;
        case NODE_LETRET:
            fprintf(fptr, "//containArray(node):%d\n",node->nodeNum);
            printAddREF(fptr,node->string, node->dataType.type, node->child); 
            break;
            //case NODE_THENSTMT:
            //case NODE_ELSESTMT:
            //    printAddREF(fptr,node->string,node->dataType.type,node->typeNode); 
            //break;
            //case NODE_FUNC_CALL:
            //    switch(node->parent->nodeNum){
            //    case NODE_LETRET:
            //    break;
            //    default:
            //    printAddREF(fptr,node->string,node->dataType.type,node); 
            //    break;
            //    }
            //break;
        default:
            break;
        }
    }

    /*
     * print DECREF 
     */
    child = node->child;
    if(child){
        do{
            if(containArray(child)){
                DecRefCountForContainedArray(fptr, child); 
            }
            child = child->rsibling;
        }while(child!=node->child);
    }

    switch(node->nodeNum){
    case NODE_THENSTMT:
    case NODE_ELSESTMT:
    case NODE_LET:
        fprintf(fptr, "\n}\n");
        break;
    }

    return;// end of sqcodegen.
}


