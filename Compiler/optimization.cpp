#include <map>
#include "optimization.h"

typedef std::map<int, int> Aliases;

////////////////////////////////////////////////////////////////////////
// Copy propagation
////////////////////////////////////////////////////////////////////////

static bool replaceAliases(Tree *n, int from, int to)
{
    bool changed = false;
    if (n->getType() == TreeType::VAR
        && n->get<int>() == from)
    {
        n->set<int>(to);
        changed = true;
    }
    for (Tree *op = n->down() ; op ; op = op->right())
    {
        if (replaceAliases(op, from, to))
        {
            changed = true;
        }
    }
    return changed;
}

static bool doCopyPropagation(CodeGraph::Node *start,
    int left, int right)
{
    bool changed = false;
    const CodeGraph::Node *next = start->getNext();
    while (next)
    {
        Tree *st = next->info;
        Tree *child = st->down();
        if (child && st->isModification())
        {
            child = child->right();
        }
        while (child)
        {
            replaceAliases(child, left, right);
            child = child->right();
        }
        next = next->getNext();
    }
    return changed;
}

static void functionCopyPropagation(Tree *func)
{
    CodeGraph *graph = createFunctionCodeGraph(func);
    bool changed = false;
    for (auto node : graph->getNodes())
    {
        Tree *st = node->info;
        Tree *left = nullptr;
        Tree *right = nullptr;
        if (st->getType() == TreeType::ASSIGN)
        {
            left = st->down();
            right = left->right()->down();
            left = left->down();
        }
        while (left && right)
        {
            if (left->getType() == TreeType::VAR
                && right->getType() == TreeType::VAR)
            {
                if (doCopyPropagation(node, left->get<int>(),
                    right->get<int>()))
                {
                    changed = true;
                }
            }
            left = left->right();
            right = right->right();
        }
    }
    delete graph;
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

////////////////////////////////////////////////////////////////////////
// Useless copy var1=var1 destroying
////////////////////////////////////////////////////////////////////////

static void removeUselessCopyingFunction(Tree *func)
{
    Tree *next;
    for (Tree *st = func->down() ; st ; st = next)
    {
        next = st->right();
        if (st->getType() == TreeType::ASSIGN)
        {
            bool equal = true;
            Tree *left = st->down();
            Tree *right = left->right()->down();
            left = left->down();
            while (left && right)
            {
                if ((left->getType() != TreeType::VAR
                    && left->getType() != TreeType::REG)
                    || right->getType() != left->getType()
                    || left->get<int>() != right->get<int>())
                {
                    break;
                }
                left = left->right();
                right = right->right();
            }
            if (left || right)
            {
                equal = false;
            }
            if (equal)
            {
                st->deleteSubtree();
            }
        }
    }
}

void removeUselessCopying(Tree *root)
{
    Tree *it = root->down();
    while (it)
    {
        removeUselessCopyingFunction(it);
        it = it->right();
    }
}

////////////////////////////////////////////////////////////////////////
// Dead code elimination
////////////////////////////////////////////////////////////////////////

static void removeDeadCodeFunc(CodeGraph *graph, Tree *func)
{
    graph->clearVisited();
    graph->dfs(graph->getNodes().front(), 0, [](CodeGraph::Node *, int){});
    for (const CodeGraph::Node *n : graph->getNodes())
    {
        if (!n->visited)
        {
            n->info->deleteSubtree();
        }
    }
}

void removeDeadCode(CodeGraphList *graphs, Tree *root)
{
    root = root->down();
    for (auto g : *graphs)
    {
        removeDeadCodeFunc(g, root);
        root = root->right();
    }
}
