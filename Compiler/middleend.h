#ifndef MIDDLEEND_H
#define MIDDLEEND_H

#include "tree.h"
#include "graph.h"

void insertReturns(Tree *root);
void createVariables(Tree *root);
Tree *makeOperations(Tree *root);
//void checkRanges(Tree *root);

#endif
