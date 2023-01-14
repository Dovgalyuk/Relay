#include "optimization.h"

void makeCopyPropagation(CodeGraphList *graph)
{
    for (auto func : *graph)
    {
        for (auto node : func->getNodes())
        {
            Tree *tree = node->info;
            if (tree->getType() == TreeType::ASSIGN)
            {
                Tree *left = tree->down();
                Tree *right = left->right()->down();
                left = left->down();
                while (left && right)
                {
                    // need SSA here, can't propagate yet
                    left = left->right();
                    right = right->right();
                }
            }
        }
    }
}
