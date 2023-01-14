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
                TreeIterator left(tree);
                left.down();
                TreeIterator right(left);
                right.right().down();
                left.down();
                while (*left && *right)
                {
                    // need SSA here, can't propagate yet
                    left.right();
                    right.right();
                }
            }
        }
    }
}
