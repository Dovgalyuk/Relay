#include <map>
#include "optimization.h"

typedef std::map<int, int> Aliases;

static int replaceAliases(Tree *n, const Aliases &aliases)
{
    int res = 0;
    if (n->getType() == TreeType::VAR
        && aliases.find(n->get<int>()) != aliases.end())
    {
        n->set<int>(aliases.at(n->get<int>()));
        ++res;
    }
    for (Tree *op = n->down() ; op ; op = op->right())
    {
        res += replaceAliases(op, aliases);
    }
    return res;
}

static void functionCopyPropagation(Tree *func)
{
    Aliases aliases;
    // collect assignments
    for (Tree *st = func->down() ; st ; st = st->right())
    {
        Tree *left = nullptr;
        Tree *right = nullptr;
        if (st->getType() == TreeType::ASSIGN)
        {
            left = st->down();
            right = left->right()->down();
            left = left->down();
        }
        else if (st->getType() == TreeType::PHI)
        {
            left = st->down();
            right = left->right();
            if (right->right())
                continue;
        }
        while (left && right)
        {
            if (left->getType() == TreeType::VAR
                && right->getType() == TreeType::VAR)
            {
                aliases[left->get<int>()] = right->get<int>();
            }
            left = left->right();
            right = right->right();
        }
    }
    // replace aliases
    while (replaceAliases(func, aliases))
    {
    }
}

void makeCopyPropagation(Tree *root)
{
    Tree *it = root->down();
    while (it)
    {
        functionCopyPropagation(it);
        it = it->right();
    }
}
