#include <map>
#include <cassert>
#include "graph.h"

CodeGraph *createFunctionGraph(Tree *func)
{
    std::map<int, CodeGraph::Node*> labels;
    std::map<Tree*, CodeGraph::Node*> nodes;

    TreeIterator it(func);
    it.down();
    CodeGraph *graph = new CodeGraph;
    // create nodes and labels
    CodeGraph::Node *prev = nullptr;
    while (*it)
    {
        CodeGraph::Node *node = graph->addNode(*it);
        if ((*it)->getType() == TreeType::LABEL)
        {
            labels[(*it)->get<int>()] = node;
        }
        nodes[*it] = node;
        if (prev)
        {
            prev->addNext(node);
        }
        prev = node;
        it.right();
    }
    // create links
    it.up().down();
    while (*it)
    {
        if ((*it)->getType() == TreeType::JMP
            || (*it)->getType() == TreeType::IF)
        {
            it.down().right();
            int label = (*it)->get<int>();
            CodeGraph::Node *labelNode = labels[label];
            assert(labelNode);
            it.up();
            CodeGraph::Node *current = nodes[*it];
            current->addNext(labelNode);
        }
        it.right();
    }
    return graph;
}

CodeGraphList *createCodeGraph(Tree *root)
{
    CodeGraphList *graph = new CodeGraphList;
    TreeIterator it(root);
    it.down();
    while (*it)
    {
        graph->push_back(createFunctionGraph(*it));
        it.right();
    }
    return graph;
}

template<>
void Graph<Tree*>::Node::printInfo()
{
    info->print(4);
}

