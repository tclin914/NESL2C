#include "node.h"
#include "symtab.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define help(s) {printf("\thelp: %s\n", s);}



struct SymTable *rootTable;
struct FuncTable *funcTable;

struct nodeType * findtype(struct nodeType *node) {
    int i = 0;
    if(typeTable->size == 0) return 0;
    for (i = 0; i < typeTable->size; i++) {
        if(isSameType( typeTable->link[i], node))
            return typeTable->link[i];
    }
    return 0;
}

struct FuncTableEntry * findFuction(char *s) {
    int size = funcTable->size;
    if(size == 0) return 0;
    for (int i = 0; i < size; i++) {
        if(!strcmp(funcTable->entries[i].name, s))
            return &funcTable->entries[i];
    }
    return 0;
}

struct SymTable * newSymTable(struct SymTable * parent) {
    struct SymTable *table = (struct SymTable*)malloc(sizeof(struct SymTable));
    table->size = 0;
    for (int i = 0; i < 100; i++) {
        strcpy(table->entries[i].name, "");
    }
    table->parent = parent;
    return table;
}

struct SymTableEntry* findSymbol(struct SymTable * SymbolTable, char *s, int mode) {
    //struct SymTableEntry * entry;
    assert(s);

    if(SymbolTable->size != 0) {
        for (int i = 0; i < SymbolTable->size; i++) {
            if(strcmp(s, SymbolTable->entries[i].name) == 0) {
                return &SymbolTable->entries[i];
            }
        }
    }
    switch(mode) {
    case FORCEDECLARE:
        return 0;
        break;
    case REFERENCE:{
        if(SymbolTable->parent != 0 && mode == REFERENCE)
            return findSymbol(SymbolTable->parent, s, mode);  
        else
            return 0;
    }
    case NOROOT:{
        if(SymbolTable->parent != rootTable) {
            return findSymbol(SymbolTable->parent, s, mode);  
        }
        else return 0;
    }
    default: assert(0);
    }
    return 0;// impossible
}

void addtype(struct nodeType *node, enum StdType type) {
    assert(node->dataType.type == type);
    assert(node->dataType.type == TYPETUPLE);
    int i = 0;
    if(!findtype(node)) {
        typeTable->link[(typeTable->size)++] = node;
    }
}


struct SymTableEntry* addVariable(char *s, enum StdType type, struct nodeType* link, int mode) {
    struct SymTable *SymbolTable = link->table;

    if(findSymbol(link->table, s, mode) != 0) {
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

    if(type == TYPETUPLE) addtype(link, type);
    printf("table:0x%p, AddVariable:%s, dataType.type:%d\n", SymbolTable, s, type);
    return &SymbolTable->entries[index];
}

void setTable(struct nodeType *node) {
    switch(node->nodeNum) {
    case NODE_NESL:
        break;

    case NODE_APPLYBODY2:
        node->child->counts = node->child->nodeNum;
        node->child->nodeNum = NODE_BODY;
    case NODE_APPLYBODY1:
    case NODE_APPLYBODY3:
    case NODE_APPLYBODY4:
    case NODE_BODY:
    case NODE_FUNC:

    case NODE_LET:{
        //printf("nodetype:%d create scope oh yeah~\n", node->nodeNum);
        struct SymTable * newScope = newSymTable(node->table);
        node->table = newScope;
        node->table->parent = node->parent->table;
        break;
    }
    default:
        assert(node->parent);
        if(node->parent->nodeNum == NODE_IN) {
            if(node->parent ->child == node) {
                if(node->parent ->parent ->parent ->nodeNum ==NODE_APPLYBODY2) {
                    assert(node->parent->parent->lsibling->nodeNum == NODE_BODY);
                    node->table = node->parent ->parent ->lsibling->table;
                    break;
                }
            }
        }

        node->table = node->parent->table;
        break;
    }
    // recursive
    struct nodeType * child = node->child;
    if(child != 0) {
        do{
            setTable(child);
            assert(child->table);
            child = child->rsibling;
        }while(child != node->child);
    }
    if(node->nodeNum == NODE_APPLYBODY2) {
        //FIXME?????
        node->child->nodeNum = node->child->counts;
        assert(node->child->table != node->child->rsibling->table);
        assert(node->child->rsibling->child->child->table !=
               node->child->rsibling->child->child->rsibling->table );
    }
}
struct nodeType* nthChild(int n, struct nodeType *node) {
    struct nodeType *child = node->child;
    for (int i = 1; i<n; i++) {
        child = child->rsibling;
    }
    return child;
}

int renamefcall(char *origin, char *new, struct nodeType *node) {
    int counts = 0;
    switch(node->nodeNum) {
    case NODE_FUNC:
        assert(0);
    case NODE_FUNC_CALL:
        // TODO
        assert(node->child->string);
        if(!strcmp(node->child->string, origin)) {
            strcpy(node->child->string, new);
        }
        printf("got one\n");
        printTree(node, 0);
        printf("###\n");
        break;
    default:
        {
            struct nodeType *child= node->child;
            if(child) {
                do{
                    counts += renamefcall(origin, new, child);
                    child = child->rsibling;
                }while(child != node->child);
            }
        }
    }
}
int isSameType( struct nodeType *L, struct nodeType *R) {
    if(L->dataType.type != R->dataType.type) return 0;
    switch(L->dataType.type) {
    case TYPEINT:
    case TYPEFLOAT:
    case TYPEBOOL:
    case TYPECHAR:
        return 1;
    case TYPESEQ:
        assert(L->typeNode->child);
        assert(R->typeNode->child);
        return isSameType(L->typeNode->child, R->typeNode->child);
    case TYPETUPLE:
        assert(L->typeNode->child->rsibling);
        assert(R->typeNode->child->rsibling);
        return isSameType(L->typeNode->child, R->typeNode->child)&&
            isSameType(L->typeNode->child->rsibling, R->typeNode->child->rsibling);
    default:
        assert(0); // impossible
    }
}

struct nodeType * removePair(struct nodeType* node) {
    struct nodeType *child = node->child; 
    struct nodeType *ignore;
    struct nodeType *pattern = node;
    struct nodeType *patright = node->rsibling;
    struct nodeType *patleft = node->lsibling;
    struct nodeType *patchild= node->child;
    struct nodeType *patparent = node->parent;
    int a = 0;

    child = node->child;
    if(!child) {return node;}
    else
    {
        do{
            a++;
            //child = removePair(child);
            child = child->rsibling;
        }while(child != node->child);
        printf("a: %d\n", a);

        for (int i = 0; i < a; i++) {
            child = removePair(child);
            child = child->rsibling;
        }

        if(node->nodeNum == NODE_TYPE_PAIR || node->nodeNum == NODE_PAIR) {
            patchild = node->child;
            assert(patchild == node->child);
            assert(patchild->rsibling == patchild);
            patchild->parent = node->parent;

            /* */
            if(patleft != node) {
                patleft->rsibling = patchild;
                node->lsibling = 0;
                patright->lsibling = patchild;
                node->rsibling = 0;
                patchild->lsibling->rsibling = patright;
                patchild->lsibling = patleft;
            }
            //patchild->rsibling = patright;
            if(patparent->child == node)
                patparent ->child = patchild;
            node = patchild;
        }
        return node;
    }
}

void tupleTransform(struct nodeType *node) {
    struct nodeType * foo  = node->child->lsibling;
    if(foo->nodeNum == NODE_TUPLE) {
        struct nodeType *c1 = nthChild(1, foo);
        struct nodeType *c2 = nthChild(2, foo);
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

void tupleBinding(struct nodeType *LHS, struct nodeType *RHS) {
    struct nodeType *lchild;
    struct nodeType *rchild;
    struct nodeType *typeNode;
    struct nodeType *ltype;
    struct nodeType *rtype;

    assert(RHS->dataType.type == TYPETUPLE);
    assert(RHS->typeNode);
    assert(RHS->typeNode->dataType.type == TYPETUPLE);

    lchild = LHS->child;
    rchild = LHS->child->rsibling;
    typeNode = RHS->typeNode;
    ltype = typeNode->child;
    rtype = typeNode->child->rsibling;
    if(LHS->nodeNum == NODE_TUPLE) {
        LHS->dataType.type = RHS->dataType.type;
        LHS->typeNode = RHS->typeNode;
    }
    if(LHS->nodeNum == NODE_TOKEN) {
        assert(0); //note implement;
    }
    // lchild of LHS
    switch(lchild->nodeNum) {
    case NODE_PAIR:{
        assert(0);    
        break;}
    case NODE_TUPLE:
        assert(lchild);
        assert(rchild);
        lchild->mode= LHS->mode;
        tupleBinding(lchild, ltype);
        lchild->dataType.type = ltype->dataType.type;
        lchild->typeNode = ltype->typeNode;
        break;
    case NODE_TOKEN:{
        struct SymTableEntry * entry;
        assert(lchild->string);
        assert(ltype->dataType.type);
        lchild->mode = LHS->mode;
        lchild->dataType.type = ltype->dataType.type;
        if(ltype->dataType.type >= TYPESEQ)
            assert(ltype->typeNode);
        lchild->typeNode = ltype->typeNode;
        entry = findSymbol(lchild->table, lchild->string, lchild->mode);
        if(entry) {
            assert(entry->type == lchild->dataType.type);
            if(entry->link->typeNode && !(lchild->typeNode))
                lchild->typeNode =  entry->link->typeNode;
        }else{
            if(!lchild->typeNode) lchild->typeNode = lchild;
            addVariable(lchild->string, lchild->dataType.type, lchild, lchild->mode);
        }
        break;
    }
    default:
        assert(0); //not possible
        break;
    }// end of lchild->nodeNum

    // rchild of LHS
    switch(rchild->nodeNum) {
    case NODE_PAIR:
        assert(0); // not implement;
        break;
    case NODE_TUPLE:
        rchild->mode= LHS->mode;
        tupleBinding(rchild, rtype);
        rchild->dataType.type = rtype->dataType.type;
        rchild->typeNode = rtype->typeNode;
        break;
    case NODE_TOKEN:{
        struct SymTableEntry * entry;
        assert(rchild->string);
        assert(rtype->dataType.type);
        rchild->mode = LHS->mode;
        rchild->dataType.type = rtype->dataType.type;
        rchild->typeNode = rtype->typeNode;
        entry = findSymbol(rchild->table, rchild->string, rchild->mode);
        if(entry) {
            assert(entry->type == rchild->dataType.type);
        }else{
            if(!rchild->typeNode) rchild->typeNode = rchild;
            addVariable(rchild->string, rchild->dataType.type, rchild, rchild->mode);
        }
        break;
    }
    default:
        assert(0); //not possible
        break;
    }// end of rchild->nodeNum
}// end of tupleBinding

void newtypeBinding(struct nodeType *node) {
    assert(node->typeNode);
    switch(node->nodeNum) {
    case NODE_TOKEN:
        node->dataType.type = node->typeNode->dataType.type;
        addVariable(node->string, node->dataType.type, node, REFERENCE);
        break;
    case NODE_TUPLE:
        node->dataType.type = node->typeNode->dataType.type;
        node->child->typeNode = node->typeNode->child;
        node->child->rsibling->typeNode = node->typeNode->child->rsibling;
        newtypeBinding(node->child);
        newtypeBinding(node->child->rsibling);
        break;
    default:
        assert(0); //not implement;
    }
}

void typeAnalysis( struct nodeType *node) {
    switch(node->nodeNum) {
    case NODE_NESL:
    case NODE_LIST:
    case NODE_RBINDS:{

        int count = 0;
        struct nodeType *child = node->child;
        if(child != 0) {
            do{
                count++;
                typeAnalysis(child);
                child = child->rsibling; 
            }while(child != node->child);
        }
        node->counts = count;
        if(node->nodeNum == NODE_RBINDS) {
            node->dataType.type = node->child->dataType.type;
            node->typeNode = node->child;
        }
        break;
    }
    case ELEM_TUPLE:
    case NODE_TUPLE:{
        typeAnalysis(node->child);
        typeAnalysis(node->child->rsibling);
        node->dataType.type = TYPETUPLE;
        node->typeNode = node;
        break;
    }

    case NODE_FUNC:{

        struct nodeType *inputParam = node->child;
        struct nodeType *typeDef = node->child->rsibling;
        struct nodeType *returnDef = typeDef->child->rsibling;
        struct nodeType *funcExp = typeDef->rsibling;
        struct FuncTableEntry *fentry;

        // functions must have the typeDefinition of Function.
        assert(typeDef->nodeNum  == NODE_OP);

        if(typeDef->op == OP_RARROW) {
            // Bind the inputParameter with TypeDeclaration
            inputParam->isParam = 1;
            typeAnalysis(typeDef->child);
            printTree(typeDef->child, 0);
            typeDef->child = removePair(typeDef->child);
            printTree(typeDef->child, 0);
            printTree(inputParam, 0);
            inputParam = removePair(inputParam);
            printTree(inputParam, 0);
            inputParam->typeNode = typeDef->child;
            newtypeBinding(inputParam);

            // Analyse the returnType of the function, RHS of op_rarrow.
            typeAnalysis(returnDef);
            node->table = node->parent->table;
            node->isParam = 1;

            printTree(returnDef, 0);
            returnDef = removePair(returnDef);
            printTree(returnDef, 0);

            node->dataType.type = returnDef->dataType.type;
            node->typeNode = returnDef->typeNode;
            if(node->dataType.type == TYPESEQ) assert(node->typeNode);

            // deal with the redefined functions.
            fentry = findFuction(node->string);
            if(fentry) {
                int ignore;
                struct nodeType *RRR = node->rsibling; 
                // already declared.
                sprintf(node->string, "%s_%d", node->string, (fentry->renametimes)++);
                ignore = renamefcall(fentry->name, node->string, node->child->rsibling->rsibling);

                printf("renamed: %d times\n", ignore);

                while(RRR!=node->parent->child) {
                    if(RRR->nodeNum == NODE_FUNC) {
                        if(!strcmp(RRR->string, fentry->name)) {
                            printf("boom~~~\n");
                            break;
                        }
                    }
                    else
                        ignore = renamefcall(fentry->name, node->string, RRR);
                    RRR = RRR->rsibling;
                    printf("renamed: %d times\n", ignore);
                }
            } 
            else{
                // first appeared.
                fentry = &funcTable->entries[funcTable->size++];
                fentry->renametimes =1;

                strcpy(fentry->name, node->string);
            }
            addVariable(node->string, typeDef->child->rsibling->dataType.type, node, REFERENCE);  

            // Assign the returnType to the functionNode
            typeAnalysis(node->child->rsibling->rsibling);
            node->dataType.type = node->child->rsibling->rsibling->dataType.type;

            // Add the function node into SymbolTable
        }
        else {
            printf("error in node_func of semanticPass\n");
            assert(0); // no function declaration.
            return;
        } 
        break;
    }

    case NODE_TYPE_SEQ:{
        typeAnalysis(node->child);
        node->typeNode = node;
        node->dataType.type = TYPESEQ;

        break;}

    case NODE_IFELSE:{
        struct nodeType* ifstmt = node->child;
        struct nodeType* thstmt = node->child->rsibling;
        struct nodeType* elstmt = node->child->rsibling->rsibling;
        typeAnalysis(ifstmt);
        typeAnalysis(thstmt);
        typeAnalysis(elstmt);
        assert(ifstmt->dataType.type == TYPEBOOL); 
        assert(thstmt->dataType.type == elstmt->dataType.type);
        node->dataType.type = elstmt->dataType.type;
        node->typeNode = elstmt->typeNode;

        break;
    }
    case NODE_IFSTMT:
    case NODE_THENSTMT:
    case NODE_ELSESTMT:
    case NODE_TYPE_PAIR:
    case NODE_PAIR:{
        typeAnalysis(node->child);
        node->dataType.type = node->child->dataType.type;
        node->typeNode = node->child->typeNode;
        break;
    } 

    case NODE_EXP:{
        typeAnalysis(node->child);
        node->dataType.type = node->child->dataType.type;
        node->typeNode = node->child->typeNode;
        break;
    }
    case NODE_LET:{
        struct nodeType *RHS= node->child->rsibling;
        typeAnalysis(node->child);
        typeAnalysis(node->child->rsibling);
        RHS = removePair(RHS);
        node->child->rsibling = RHS;
        RHS->rsibling = node->child;
        node->dataType.type = node->child->rsibling->dataType.type;
        node->typeNode = node->child->rsibling->typeNode;

        break;
    }
    case NODE_BIND:{
        struct nodeType *child = node->child;
        int counts=0;
        if(child!=0) {
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
        struct SymTableEntry * entry = findSymbol(node->child->table, node->child->string, REFERENCE);
        struct nodeType *typerefNode;
        struct nodeType *RHS = node->child->rsibling;

        // analyse the index part.
        typeAnalysis(RHS);
        assert(RHS->dataType.type == TYPEINT);

        // analyse the sequence.
        assert(entry);      
        assert(entry->type == TYPESEQ);
        assert(entry->link);
        typerefNode = entry->link;
        assert(typerefNode->typeNode);
        typerefNode = typerefNode->typeNode;
        assert(typerefNode->child);
        node->dataType.type = typerefNode->child->dataType.type;
        node->typeNode = typerefNode->child;
        if(typerefNode->child->typeNode) node->typeNode = typerefNode->child->typeNode;
        assert(node->typeNode);
        break;
    }

    case NODE_ASSIGN:{
        struct nodeType *LHS= node->child;
        struct nodeType *RHS=LHS->rsibling;
        struct nodeType * pattern = LHS;
        struct nodeType * pchild = pattern ->child;

        typeAnalysis(RHS);
        assert(RHS->dataType.type);
        if(RHS->dataType.type == TYPETUPLE) {
            assert(RHS->typeNode);
            printTree(RHS, 0);
            RHS = removePair(RHS);
            printTree(RHS, 0);
            assert(RHS->typeNode);
        }

        LHS->dataType.type = RHS->dataType.type;
        LHS->typeNode = RHS->typeNode;

        /*Remove Pattern*/
        if(pattern->nodeNum == NODE_PATTERN) {
            node->mode = NOROOT;
            struct nodeType *patright = pattern->rsibling;
            struct nodeType *patleft = pattern->lsibling;
            struct nodeType *patchild= pattern->child;
            struct nodeType *patparent = pattern->parent;

            do{
                patchild->parent = patparent;
                patchild = patchild->rsibling;
            }while(patchild!=pattern->child);


            patleft->rsibling = patchild;
            patright->lsibling = patchild;
            patparent ->child = patchild;
            pattern = patchild;
            assert(pattern->lsibling == pattern);
            assert(pattern->rsibling == pattern);
            pattern->rsibling = patright;
            pattern->lsibling = patleft;
        }

        /* assign pchild */
        pchild = pattern->child;

        /*remove pair inside pchild*/
        printTree(pattern, 0);
        pattern = removePair(pattern);
        printTree(pattern, 0);

        /* top-level need to be declared in global */
        if(node->parent->nodeNum == NODE_NESL)
            pattern->isParam = 0;

        /* handleing */
        switch(pattern->nodeNum) {
        case NODE_TOKEN:{
            struct SymTableEntry * entry;
            assert(pattern->string);
            assert(RHS->dataType.type);
            pattern->dataType.type=RHS->dataType.type;
            pattern->typeNode=RHS->typeNode;
            entry = findSymbol(pattern->table, pattern->string, REFERENCE);
            if(!entry) {
                addVariable(pattern->string, pattern->dataType.type, pattern, REFERENCE);
            }
            break;}
        case NODE_TUPLE:
            pattern->mode = node->mode;
            pattern->dataType.type = RHS->dataType.type;
            pattern->typeNode = RHS->typeNode;
            tupleBinding(pattern, RHS);
            break;
        default:
            assert(0);
        }
        node->dataType.type = RHS->dataType.type;
        node->typeNode=RHS->typeNode;
        break;
    }
    case NODE_OP:{
        struct nodeType* LHS = node->child;
        struct nodeType* RHS = node->child->rsibling;
        typeAnalysis(LHS);
        if(RHS!=LHS)
            typeAnalysis(RHS);
        switch(node->op) {
        case OP_ADD:
            assert(LHS->dataType.type == RHS->dataType.type);
            node->dataType.type = RHS->dataType.type;
            assert(node->dataType.type<=TYPEFLOAT);
            break;
        case OP_SUB:
            assert(LHS->dataType.type == RHS->dataType.type);
            node->dataType.type = RHS->dataType.type;
            break;
        case OP_MUL:
            assert(LHS->dataType.type == RHS->dataType.type);
            node->dataType.type = RHS->dataType.type;
            break;
        case OP_DIV:
            assert(LHS->dataType.type == RHS->dataType.type);
            node->dataType.type = RHS->dataType.type;
            break;
        case OP_RARROW:
            assert(LHS->dataType.type == RHS->dataType.type);
            break;
        case OP_AT:
            break;
        case OP_UMINUS:
            node->dataType.type=LHS->dataType.type;
            node->typeNode = LHS->typeNode;
            break;
        case OP_SHARP:{
            if(LHS->dataType.type ==TYPESEQ ) {
                node->dataType.type = TYPEINT;
            }else{ 
                assert(0); // semantic error
            }
            break;
        }
        case OP_GT:
        case OP_EQ:
        case OP_GE:
        case OP_LE:
        case OP_NE:
        case OP_LT:
            assert(LHS->dataType.type == RHS->dataType.type);
            node->dataType.type = TYPEBOOL;
            break;
        case OP_NOT:
            node->dataType.type = LHS->dataType.type;
            break;
        case OP_COMMA:
            break;
        case OP_OR:
        case OP_NOR:
        case OP_XOR:
        case OP_AND:
        case OP_NAND:
            assert(LHS->dataType.type == RHS->dataType.type);
            node->dataType.type = RHS->dataType.type;
            break;
        case OP_LARROW:
            assert(0);
            break;
        case OP_UPT:
            assert(RHS->dataType.type == TYPEINT);
            node->dataType.type = LHS->dataType.type;
            break;
        case OP_PP:{
            struct nodeType *LHSchild, *RHSchild;
            int done = 0;
            assert(LHS->dataType.type == TYPESEQ);
            assert(RHS->dataType.type == TYPESEQ);
            LHSchild = LHS->typeNode->child;
            RHSchild = RHS->typeNode->child;
            assert(isSameType(LHSchild, RHSchild));
            node->dataType.type = LHS->dataType.type;
            node->typeNode = LHS->typeNode;
            break;
        }
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
        RHS = removePair(RHS);

        // second, check the function return type.
        assert(node->child->nodeNum == NODE_TOKEN);
        assert(node->child->string);

        if(strcmp(node->child->string, "isContiguousList") == 0) {
            node->dataType.type = TYPEINT;
            return;
        }
        else if(strcmp(node->child->string, "verifyQHull") == 0) {
            struct nodeType *param1 = RHS->child;
            struct nodeType *param2 = param1->rsibling;
            node->dataType.type= TYPEBOOL;
            assert(param1->dataType.type == TYPEINT); 
            assert(param2->dataType.type == TYPESEQ); 
            return;
        }
        else if(strcmp(node->child->string, "dist") == 0) {
            struct nodeType *typer = newNode(NODE_TYPE_SEQ);
            typer->child = RHS->child;
            if(RHS->child->typeNode) typer->child = RHS->child->typeNode;
            typer->dataType.type = TYPESEQ;
            node->dataType.type = TYPESEQ;
            node->typeNode = typer;
            return;
        }
        else if(strcmp(node->child->string, "time") == 0) {
            // sequential version
            node->dataType.type = TYPETUPLE;
            struct nodeType *refNode = newNode(NODE_TYPE_TUPLE);
            struct nodeType *Lchild = newNode(NODE_TOKEN);
            struct nodeType *Rchild = newNode(NODE_TOKEN);
            addChild(refNode, Lchild);
            addChild(refNode, Rchild);
            Rchild->token = TOKE_FLOAT;
            Rchild->dataType.type = TYPEFLOAT;
            Rchild->table = node->table;
            refNode->table = node->table;
            refNode->dataType.type = TYPETUPLE;
            refNode->typeNode = refNode;
            assert(node->child->rsibling->dataType.type);
            Lchild->table = node->table;
            Lchild->token = -1;
            Lchild->dataType.type = node->child->rsibling->dataType.type;
            Lchild->typeNode = node->child->rsibling->typeNode;
            node->typeNode = refNode;
            typeAnalysis(refNode);
            node->dataType.type = refNode->dataType.type;
            return;
        }else if(strcmp(node->child->string, "rand") == 0) {
            node->dataType.type = RHS->dataType.type;
            return;
        }else if(strcmp(node->child->string, "plusp") == 0) {
            node->dataType.type = TYPEBOOL;
            assert(RHS->dataType.type >= TYPEINT && RHS->dataType.type <= TYPEFLOAT);
            return;
        }else if(strcmp(node->child->string, "flatten") == 0) {
            assert(RHS->dataType.type == TYPESEQ);
            assert(RHS->typeNode->child->dataType.type == TYPESEQ);
            node->dataType.type = TYPESEQ;
            //node->typeNode = RHS->typeNode->child;
            node->typeNode = RHS->typeNode->child->typeNode;
            //if(RHS->typeNode->child) node->typeNode = RHS->typeNode->child->child;
            return;
        }else if(strcmp(LHS->string, "max_index") == 0) {
            node->dataType.type = TYPEINT;
            //RHS=removePair(RHS);
            assert(RHS->dataType.type == TYPESEQ);
            return;
        }else if(strcmp(LHS->string, "min_index") == 0) {
            node->dataType.type = TYPEINT;
            //RHS=removePair(RHS);
            assert(RHS->dataType.type == TYPESEQ);
            return;
        }else if(strcmp(LHS->string, "float") == 0) {
            node->dataType.type = TYPEFLOAT;
            return;
        }else if(strcmp(LHS->string, "round") == 0) {
            node->dataType.type = TYPEINT;
            assert(RHS->dataType.type == TYPEFLOAT);
            return;
        }else if(strcmp(LHS->string, "print_string") == 0) {
            node->dataType.type = TYPEBOOL;
            //assert(RHS->dataType.type == TYPESEQ_C);
            return;
        }else if(strcmp(LHS->string, "genShuffledList") == 0) {
            struct nodeType *refNode1 = newNode(NODE_TYPE_SEQ);
            struct nodeType *refNode2 = newNode(NODE_TYPE_INT);
            addChild(refNode1, refNode2);
            refNode1->dataType.type = TYPESEQ;
            refNode2->dataType.type = TYPEINT;
            node->dataType.type = TYPESEQ;
            node->typeNode = refNode1;
            assert(RHS->dataType.type == TYPETUPLE);
            return;
        }else if(strcmp(LHS->string, "genReverseList") == 0) {
            struct nodeType *refNode1 = newNode(NODE_TYPE_SEQ);
            struct nodeType *refNode2 = newNode(NODE_TYPE_INT);
            addChild(refNode1, refNode2);
            refNode1->dataType.type = TYPESEQ;
            refNode2->dataType.type = TYPEINT;
            node->dataType.type = TYPESEQ;
            node->typeNode = refNode1;
            assert(RHS->dataType.type == TYPEINT);
            return;
        }else if(strcmp(LHS->string, "sum") == 0) {
            switch(RHS->typeNode->child->dataType.type) {
            case TYPEINT:
                node->dataType.type = TYPEINT;
                break;
            case TYPEFLOAT:
                node->dataType.type = TYPEFLOAT;
                break;
            default:
                assert(0); //not implement;
                break;
            }

            return;
        }else if(strcmp(LHS->string, "mod") == 0) {

            if(RHS->dataType.type != TYPETUPLE) {
                printf("***semantic check error***\n");
                abort();
            }
            node->dataType.type = TYPEINT;
            LHS->dataType.type = TYPEINT;
            return;
        }else{
            // TODO other built-in functions
            typeAnalysis(node->child);
            struct SymTableEntry *entry = findSymbol(node->table, node->child->string, REFERENCE); 
            assert(entry);
            assert(entry->type);
            node->dataType.type = entry->type;
            node->typeNode = entry->link->typeNode; 
        }
        // 1. search the node->child->string in built-in list
        //    if found then use the signature to check and 
        //    assign the type informations.
        // 2. if not found, then search the symbol table
        //    it should be a user define function.

        //node->dataType.type = node->child->dataType.type;
        break;
    }// end of NODE_FUNC_CALL

    case NODE_APPLYBODY1:{
        typeAnalysis(node->child);
        node->dataType.type = TYPESEQ;
        node->typeNode = node->child->child->child->rsibling;
        break;
    }
    case NODE_APPLYBODY2:{
        typeAnalysis(node->child->rsibling);
        typeAnalysis(node->child);
        node->dataType.type = TYPESEQ;
        node->typeNode = node;
        break;
    }
    case NODE_APPLYBODY3:{
        typeAnalysis(node->child);
        typeAnalysis(node->child->rsibling);
        assert(node->child->rsibling->dataType.type == TYPEBOOL);
        assert(node->child->dataType.type);
        node->dataType.type = TYPESEQ;
        assert(node->child->child->child->rsibling->dataType.type == TYPESEQ);
        assert(node->child->child->child->rsibling->typeNode);
        node->typeNode = node->child->child->child->rsibling->typeNode;
        break;
    }
    case NODE_APPLYBODY4:{
        typeAnalysis(node->child->rsibling);
        typeAnalysis(node->child->lsibling);
        typeAnalysis(node->child);
        node->dataType.type = TYPESEQ;
        node->typeNode = node->child->rsibling->child->child->rsibling->typeNode;
        assert(node->child->lsibling->dataType.type == TYPEBOOL);
        break;
    }
    case NODE_IN:{
        struct nodeType* LHS = node->child;
        struct nodeType* RHS = node->child->rsibling;

        // TODO e in a; or (x, y) in points; ... ; 
        // so we need to removePair(LHS); 
        // then bind the child of the RHS's typeNode with LHS.

        typeAnalysis(RHS);
        assert(RHS->dataType.type == TYPESEQ);
        assert(RHS->typeNode);
        assert(RHS->typeNode->dataType.type);
        node->dataType.type = RHS->dataType.type;
        node->typeNode = RHS->typeNode;
        LHS = removePair(LHS);

        // FIXME make tupleBinding can handle this
        if(LHS->nodeNum == NODE_TUPLE) {
            tupleBinding(LHS, RHS->typeNode->child);
            LHS->dataType.type = RHS->typeNode->child->dataType.type;
            LHS->typeNode = RHS->typeNode->child;
            if(RHS->typeNode->child->typeNode)
                LHS->typeNode = RHS->typeNode->child->typeNode;
            LHS->nodeNum = RB_TUPLE;
            typeAnalysis(LHS);
        }else{
            LHS->dataType.type = RHS->typeNode->child->dataType.type;
            LHS->typeNode = RHS->typeNode->child;
            if(RHS->typeNode->child->typeNode)
                LHS->typeNode = RHS->typeNode->child->typeNode;
            if(LHS->nodeNum == NODE_TOKEN) {
                addVariable(LHS->string, LHS->dataType.type, LHS, REFERENCE); 
            }
        }
        assert(LHS->dataType.type); 
        assert(LHS->typeNode);
        break;
    }// end of NODE_IN

    case RB_TUPLE:{
        struct nodeType* LHS = node->child;
        struct nodeType* RHS = node->child->rsibling;
        struct nodeType* lref = node->typeNode->child;
        struct nodeType* rref = lref->rsibling;

        if(LHS->nodeNum == NODE_TUPLE) {
            LHS->nodeNum = RB_TUPLE;
            typeAnalysis(LHS);
        }else if(LHS->nodeNum == NODE_TOKEN) {
            if(!findSymbol(LHS->table, LHS->string, REFERENCE)) {
                addVariable(LHS->string, lref->dataType.type, LHS, FORCEDECLARE);
                LHS->dataType.type = lref->dataType.type;
                LHS->typeNode = lref;
            }
        }else{
            assert(0);
        }

        if(RHS->nodeNum == NODE_TUPLE) {
            RHS->nodeNum = RB_TUPLE;
            typeAnalysis(RHS);
        }else if(RHS->nodeNum == NODE_TOKEN) {
            if(!findSymbol(RHS->table, RHS->string, REFERENCE)) {
                addVariable(RHS->string, rref->dataType.type, RHS, FORCEDECLARE);
                RHS->dataType.type = rref->dataType.type;
                RHS->typeNode = rref;
            }
        }else{
            assert(0);
        }

        break;
    }// end of RB_TUPLE

    case NODE_TOKEN: {
        switch(node->token) {
        case TOKE_ID:{
            struct SymTableEntry* entry = findSymbol(node->table, node->string, REFERENCE);
            if(entry) {
                node->dataType.type = entry->type; 
                node->typeNode = entry->link->typeNode;

                if(node->dataType.type == TYPESEQ) {
                    node->typeNode = entry->link->typeNode;
                }
                //addVariable(node->string, TYPEINT, node);
            }else{
                printf("###[Warning] maybe not declare variable: %s ###\n", node->string);
            }
            break;
        }
        case TOKE_INT:
            node->dataType.type = TYPEINT;
            break;
        case TOKE_FLOAT:
            node->dataType.type = TYPEFLOAT;
            break;
        case TOKE_BOOL:
            node->dataType.type = TYPEBOOL;
            break;
        case TOKE_CHAR:
            node->dataType.type = TYPECHAR;
            break;
        case TOKE_STRING:{
            struct nodeType* refNode = newNode(NODE_CHAR);
            refNode->dataType.type = TYPECHAR;
            refNode->typeNode = refNode;
            node->dataType.type = TYPESEQ;
            node->typeNode = refNode;
            break;}
        default:
            break;
        }// end of switch token
        break;
    }// end of NODE_TOKEN

    case NODE_INT:{
        node->dataType.type = TYPEINT;
        break;
    }
    case NODE_BOOL:{
        node->dataType.type = TYPEBOOL;
        if(strcmp(node->string, "f") == 0) {
            node->iValue = 0;
        }else if(strcmp(node->string, "F") == 0) {
            node->iValue = 0;
        }else if(strcmp(node->string, "t") == 0) {
            node->iValue = 1;
        }else if(strcmp(node->string, "T") == 0) {
            node->iValue = 1;
        }

        break;
    }
    case NODE_FLOAT:{
        node->dataType.type = TYPEFLOAT;
        node->typeNode = node;
        break;
    }
    case NODE_CHAR:{
        node->dataType.type = TYPECHAR;
        node->typeNode = node;
        break;
    }
    case NODE_STRING:{
        struct nodeType* refNode = newNode(NODE_CHAR);
        refNode->dataType.type = TYPECHAR;
        refNode->typeNode = refNode;
        node->dataType.type = TYPESEQ;
        node->typeNode = refNode;
        break;}

    case NODE_FILTER:{
        int count = 0;
        struct nodeType *child = node->child;
        if(child) {
            do{
                count++;
            }while(child!= node->child);
        }
        node->counts = count;
        typeAnalysis(node->child);
        node->dataType.type = node->child->dataType.type;
        node->typeNode = node;
        break;
    }
    case NODE_NEW_SEQ:{
        struct nodeType *LHS = node->child;
        struct nodeType *RHS = node->child->rsibling;
        int count = 0;
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
        if(LHS->nodeNum == NODE_PAIR && LHS->child->nodeNum == NODE_TUPLE) {
            // remove pair;
            struct nodeType * child = LHS; //child = node->child;
            struct nodeType * rhs = LHS->rsibling;
            struct nodeType * lhs = LHS->lsibling;
            struct nodeType * pchild = LHS->child;
            LHS->child->nodeNum = ELEM_TUPLE; 
            while(child->nodeNum == NODE_PAIR) {
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

        // below consider no pair_node in tree 
        typeAnalysis(LHS);
        LHS->paramcount = 0;
        if(RHS->nodeNum == NODE_TUPLE) {
            RHS->nodeNum = NODE_SEQ_TUPLE;
            RHS->paramcount = 1;
            typeAnalysis(RHS);
            assert(LHS->dataType.type == RHS->dataType.type);
            node->counts = 1+RHS->counts;
        }
        else{
            if(RHS->nodeNum == NODE_PAIR && RHS->child->nodeNum == NODE_TUPLE) {
                // remove pair;
                struct nodeType * child = RHS; //child = node->child;
                struct nodeType * rhs = RHS->rsibling;
                struct nodeType * lhs = RHS->lsibling;
                struct nodeType * pchild = RHS->child;
                RHS->child->nodeNum = ELEM_TUPLE;
                while(child->nodeNum == NODE_PAIR) {
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
            typeAnalysis(RHS);
            RHS->paramcount = 1;
        }// end of else.

        switch(LHS->dataType.type) {
        case TYPEINT: 
        case TYPEFLOAT:
        case TYPEBOOL:
        case TYPECHAR:
        default:
            node->dataType.type = TYPESEQ;
            node->typeNode = node;
            break;
        }
        break;
    }

    case NODE_SEQ_TUPLE:{
        struct nodeType* LHS = node->child;
        struct nodeType* RHS = node->child->rsibling;

        if(LHS->nodeNum == NODE_PAIR && LHS->child->nodeNum == NODE_TUPLE) {
            // remove pair;
            struct nodeType * child = LHS; //child = node->child;
            struct nodeType * rhs = LHS->rsibling;
            struct nodeType * lhs = LHS->lsibling;
            struct nodeType * pchild = LHS->child;

            LHS->child->nodeNum = ELEM_TUPLE;
            while(child->nodeNum == NODE_PAIR) {
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
        if(RHS->nodeNum == NODE_PAIR && RHS->child->nodeNum == NODE_TUPLE) {
            // remove pair;
            struct nodeType * child = RHS; //child = node->child;
            struct nodeType * rhs = RHS->rsibling;
            struct nodeType * lhs = RHS->lsibling;
            struct nodeType * pchild = RHS->child;
            RHS->child->nodeNum = ELEM_TUPLE;
            while(child->nodeNum == NODE_PAIR) {
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
        node->dataType.type = LHS->dataType.type;
        LHS->paramcount = node->paramcount;
        if(RHS->nodeNum == NODE_TUPLE) {
            RHS->nodeNum = NODE_SEQ_TUPLE;
            RHS->paramcount = node->paramcount + 1;
            typeAnalysis(RHS);
            assert(LHS->dataType.type == RHS->child->dataType.type);
            node->counts = RHS->counts+1;
        }else{
            typeAnalysis(RHS);
            assert(LHS->dataType.type == RHS->dataType.type);
            node->counts = 2;
            RHS->paramcount = node->paramcount +1;
        }
        node->typeNode=node;
        break;
    } 
    case NODE_SEQ:{
        struct nodeType * child= node->child;
        assert(child);
        typeAnalysis(child);
        switch(child->dataType.type) {
        case TYPEINT:
        case TYPEFLOAT:
        case TYPEBOOL:
        case TYPECHAR:
        default:
            node->dataType.type = TYPESEQ;
            node->typeNode = node;
            break;
        }

        break;
    }

    }// end of switch nodeType

}//end of typeAnalysis

//void setTable(struct nodeType *node, struct SymTable *table) {


void semanticPass( struct nodeType *node) {
    // create and set a table in rootNode. 
    // then start the Analysis
    // which consist of table insertion, 
    // typeBinding and typeChechking.

    /*initialize the function table*/
    typeTable = (struct TypeTable*)malloc(sizeof(struct TypeTable));
    typeTable->size = 0;

    funcTable = (struct FuncTable*)malloc(sizeof(struct FuncTable));
    funcTable->size = 0;
    for (int i = 0; i < 100; i++) {
        funcTable->entries[i].renametimes = 0;
        strcpy(funcTable->entries[i].name, "");
    }

    rootTable = newSymTable(NULL);
    node->table = rootTable;
    setTable(node);
    typeAnalysis(node);
}
