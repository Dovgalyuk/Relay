#include "objects.h"

static int temps = 1000;
static int labels = 1;

Tree *createTemp()
{
    return new Tree(temps++, TreeType::VAR);
}

Tree *createTempVar(Tree *list1, Tree *list2)
{
    Tree *res = new Tree(TreeType::LIST);
    Tree *it1 = list1->down();
    Tree *it2 = list2 ? list2->down() : nullptr;
    while (it1 || it2)
    {
        res->addChild(createTemp());
        if (it1)
            it1 = it1->right();
        if (it2)
            it2 = it2->right();
    }
    return res;
}

Tree *createLabel()
{
    return new Tree(labels++, TreeType::LABEL);
}
