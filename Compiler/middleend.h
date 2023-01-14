#ifndef MIDDLEEND_H
#define MIDDLEEND_H

#include "tree.h"
#include "graph.h"

void createVariables(CodeGraphList *graphs, Tree *root);
Tree *makeOperations(Tree *root);

#endif
