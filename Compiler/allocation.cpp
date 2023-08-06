#include <map>
#include <set>
#include "allocation.h"
#include "graph.h"

typedef std::set<int> Vars;
typedef std::map<Tree *, Vars> NodeVars;
typedef Graph<int> VariableGraph;
typedef VariableGraph::Node VariableNode;
typedef std::map<int, VariableNode*> VariableNodes;

static VariableNode *variableNode(int var, VariableNodes &nodes, VariableGraph *graph)
{
    VariableNode *node = nodes[var];
    if (!node)
    {
        node = graph->addNode(var);
        nodes[var] = node;
    }
    return node;
}

static void colorGraph(VariableGraph *graph)
{
    for (VariableNode *n : graph->getNodes())
    {
        if (!n->visited)
        {
            graph->dfs(n, 0, [](VariableNode *node, int unused) {
                if (node->visited)
                {
                    return;
                }
                Vars colors;
                for (VariableNode *link : node->links)
                {
                    if (link->visited)
                    {
                        colors.insert(link->color);
                    }
                }
                int i = 1;
                for (int c : colors)
                {
                    if (c == i)
                    {
                        i = c + 1;
                    }
                    else if (c > i)
                    {
                        break;
                    }
                }
                node->color = i;
            });
        }
    }
}

static VariableGraph *buildVariableGraph(const NodeVars &in, const NodeVars &out)
{
    VariableNodes nodes;
    VariableGraph *graph = new VariableGraph;
    for (auto nv : in)
    {
        const Vars &v = nv.second;
        for (int var : v)
        {
            VariableNode *node1 = variableNode(var, nodes, graph);
            // add links
            for (int var2 : v)
            {
                if (var != var2)
                {
                    VariableNode *node2 = variableNode(var2, nodes, graph);
                    node1->addNext(node2);
                    node2->addNext(node1);
                }
            }
        }
    }
    return graph;
}

static void setRegisters(Tree *func, VariableGraph *graph)
{
    std::map<int, int> varToReg;
    for (auto v : graph->getNodes())
    {
        varToReg[v->info] = v->color;
    }
    for (Tree *stat = func->down() ; stat ; stat = stat->right())
    {
        Tree *prev = nullptr;
        Tree *right = stat->down();
        for ( ; right ; right = right->right())
        {
            if (right->getType() == TreeType::LIST)
            {
                assert(!prev);
                prev = right;
                right = right->down();
            }
            if (right->getType() == TreeType::VAR)
            {
                right->setType(TreeType::REG);
                right->set<int>(varToReg[right->get<int>()]);
            }
            if (prev && !right->right())
            {
                right = prev;
                prev = nullptr;
            }
        }
    }
}

void simpleAllocationFunction(Tree *func)
{
    // NodeVars liveVars;
    NodeVars defVars;
    NodeVars useVars;
    NodeVars outVars;
    NodeVars inVars;
    // NodeVars defPhiVars;
    // NodeVars usePhiVars;
    // fill initial liveVars and defVars
    for (Tree *stat = func->down() ; stat ; stat = stat->right())
    {
        Tree *right = stat->down();
        // bool isPhi = stat->getType() == TreeType::PHI;
        // definitions
        if (stat->isModification()
            && stat->down())
        {
            Tree *left = stat->down();
            right = right->right();
            bool one = left->getType() != TreeType::LIST;
            if (!one)
            {
                left = left->down();
            }
            for ( ; left ; left = left->right())
            {
                if (left->getType() == TreeType::VAR)
                {
                    // if (isPhi)
                    // {
                    //     defPhiVars[stat].insert(left->get<int>());
                    // }
                    // else
                    {
                        defVars[stat].insert(left->get<int>());
                    }
                }
                if (one)
                    break;
            }
        }
        // usages
        Tree *prev = nullptr;
        for ( ; right ; right = right->right())
        {
            if (right->getType() == TreeType::LIST)
            {
                assert(!prev);
                prev = right;
                right = right->down();
            }
            if (right->getType() == TreeType::VAR)
            {
                // if (isPhi)
                // {
                //     usePhiVars[stat].insert(right->get<int>());
                // }
                // else
                {
                    useVars[stat].insert(right->get<int>());
                    inVars[stat].insert(right->get<int>());
                }
            }
            if (prev && !right->right())
            {
                right = prev;
                prev = nullptr;
            }
        }
    }
    // bool changed;
    // // move down from definitions
    // NodeVars reachVars = defVars;
    // reachVars.insert(defPhiVars.begin(), defPhiVars.end());
    // CodeGraph *graph = createCodeGraph(func);
    // do
    // {
    //     changed = false;
    //     // propagate reachVars
    //     graph->clearVisited();
    //     CodeGraph::Node *node = graph->getNodes().front();
    //     graph->dfs(node, (CodeGraph::Node*)nullptr,
    //         [&reachVars, &changed]
    //             (CodeGraph::Node *node, CodeGraph::Node *&prev)
    //         {
    //             Tree *t = node->info;
    //             Tree *pt = prev ? prev->info : nullptr;
    //             const Vars &reachPrev = reachVars[pt];
    //             Vars &reach = reachVars[t];
    //             for (auto var : reachPrev)
    //             {
    //                 if (!reach.contains(var))
    //                 {
    //                     reach.insert(var);
    //                     changed = true;
    //                 }
    //             }
    //             prev = node;
    //         }
    //     );
    // }
    // while (changed);
    // delete graph;

    bool changed;
    // move up from usage
    CodeGraph *graph = createCodeGraph(func);
    do
    {
        changed = false;
        // propagate in & out
        graph->clearVisited();
        CodeGraph::Node *node = graph->getNodes().front();
        graph->dfs(node, 0,
            [&defVars, &useVars, &inVars, &outVars, &changed]
                (CodeGraph::Node *node, int unused)
            {
                Tree *t = node->info;
                Vars &out = outVars[t];
                for (CodeGraph::Node *link : node->links)
                {
                    Tree *next = link->info;
                    for (auto var : inVars[next])
                    {
                        if (!out.contains(var))
                        {
                            out.insert(var);
                            changed = true;
                        }
                    }
                }

                Vars add = out;
                for (auto def : defVars[t])
                {
                    add.erase(def);
                }

                Vars &in = inVars[t];
                for (auto var : add)
                {
                    if (!in.contains(var))
                    {
                        in.insert(var);
                        changed = true;
                    }
                }
            }
        );
    }
    while (changed);
    delete graph;

    // debug output
    std::cout << "=== After liveness analysis:\n";
    for (Tree *stat = func->down() ; stat ; stat = stat->right())
    {
        std::cout << "In:";
        for (auto var : inVars[stat])
            std::cout << " " << var;
        std::cout << "\nOut:";
        for (auto var : outVars[stat])
            std::cout << " " << var;
        std::cout << "\n";
        stat->print(2);
    }

    // build variable graph
    VariableGraph *varGraph = buildVariableGraph(inVars, outVars);

    // allocate registers
    colorGraph(varGraph);
    setRegisters(func, varGraph);

    std::cout << "=== Variable graph:\n";
    varGraph->print();

    delete varGraph;
}

void simpleAllocation(Tree *root)
{
    // iterate over functions
    Tree *it = root->down();
    while (it)
    {
        simpleAllocationFunction(it);
        it = it->right();
    }
}
