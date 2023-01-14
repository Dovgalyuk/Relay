#include <map>
#include <cassert>
#include "graph.h"

CodeGraph *createFunctionGraph(Tree *func)
{
    std::map<int, CodeGraph::Node*> labels;
    std::map<Tree*, CodeGraph::Node*> nodes;

    Tree *it = func->down();
    CodeGraph *graph = new CodeGraph;
    // create nodes and labels
    CodeGraph::Node *prev = nullptr;
    while (it)
    {
        CodeGraph::Node *node = graph->addNode(it);
        if (it->getType() == TreeType::LABEL)
        {
            labels[it->get<int>()] = node;
        }
        nodes[it] = node;
        if (prev && prev->info)
        {
            TreeType pt = prev->info->getType();
            if (pt != TreeType::GOTO && pt != TreeType::RETURN)
            {
                prev->addNext(node);
            }
        }
        prev = node;
        it = it->right();
    }
    // create links
    it = func->down();
    while (it)
    {
        if (it->getType() == TreeType::JMP
            || it->getType() == TreeType::IF
            || it->getType() == TreeType::GOTO)
        {
            Tree *lab = it->down();
            if (lab->right())
                lab = lab->right();
            int label = lab->get<int>();
            CodeGraph::Node *labelNode = labels[label];
            assert(labelNode);
            CodeGraph::Node *current = nodes[it];
            current->addNext(labelNode);
        }
        it = it->right();
    }
    return graph;
}

CodeGraphList *createCodeGraph(Tree *root)
{
    CodeGraphList *graph = new CodeGraphList;
    Tree *it = root->down();
    while (it)
    {
        graph->push_back(createFunctionGraph(it));
        it = it->right();
    }
    return graph;
}

template<>
void Graph<Tree*>::Node::printInfo()
{
    info->print(4);
}
