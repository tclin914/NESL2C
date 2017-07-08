#ifndef __SEMANTIC_PASS_H__
#define __SEMANTIC_PASS_H__

#include "treenode.h"

int semantic_pass(treenode *node);

static int check_logic_op(treenode *node);
static int check_relational_op(treenode *node);
static int check_arithmetic_op(treenode *node);
static int check_pp(treenode *node);

#endif

