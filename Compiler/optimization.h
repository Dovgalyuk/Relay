#ifndef OPTIMIZATION_H
#define OPTIMIZATION_H

#include "tree.h"
#include "graph.h"

void removeDeadCode(CodeGraphList *graphs, Tree *root);
void makeCopyPropagation(Tree *root);
void removeUselessCopying(Tree *root);

#endif
