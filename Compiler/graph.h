#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <list>
#include "tree.h"

template<typename T>
class Graph
{
public:
    class Node;
    typedef std::list<Node *> Nodes;

    class Node
    {
    public:
        friend class Graph;

        explicit Node(const T &i)
            : info(i), visited(false)
        {
        }

        void printInfo()
        {
        }

        void addNext(Node *n)
        {
            links.push_back(n);
        }

        bool visited;
        T info;
    private:
        Nodes links;
    };

    Node *addNode(const T &i)
    {
        Node *n = new Node(i);
        nodes.push_back(n);
        return n;
    }

    void clearVisited()
    {
        for (auto n : nodes)
        {
            n->visited = false;
        }
    }

    void print()
    {
        for (auto n : nodes)
        {
            std::cout << "Node " << n << " ->";
            for (auto next : n->links)
            {
                std::cout << " " << next;
            }
            std::cout << "\n";
            n->printInfo();
        }
    }

    template <typename S, typename F>
    void dfs(Node *n, S state, const F &func)
    {
        func(n, state);
        if (!n->visited)
        {
            n->visited = true;
            for (auto next : n->links)
                dfs(next, state, func);
        }
    }

    const Nodes &getNodes() const { return nodes; }

private:
    Nodes nodes;
};

typedef Graph<Tree*> CodeGraph;
typedef std::list<CodeGraph*> CodeGraphList;

CodeGraphList *createCodeGraph(Tree *root);

#endif
