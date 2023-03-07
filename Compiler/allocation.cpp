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
                Vars colors;
                for (VariableNode *link : node->links)
                {
                    if (link->visited)
                    {
                        colors.insert(link->color);
                    }
                }
            });
        }
    }
}

VariableGraph *buildVariableGraph(const NodeVars &live)
{
    VariableNodes nodes;
    VariableGraph *graph = new VariableGraph;
    for (auto nv : live)
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

void simpleAllocationFunction(Tree *func)
{
    NodeVars liveVars;
    NodeVars defVars;
    NodeVars defPhiVars;
    NodeVars usePhiVars;
    // fill initial liveVars and defVars
    for (Tree *stat = func->down() ; stat ; stat = stat->right())
    {
        Tree *right = stat->down();
        bool isPhi = stat->getType() == TreeType::PHI;
        // definitions
        if (stat->getType() != TreeType::CMP
            && stat->getType() != TreeType::RETURN
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
                    if (isPhi)
                    {
                        defPhiVars[stat].insert(left->get<int>());
                    }
                    else
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
                if (isPhi)
                {
                    usePhiVars[stat].insert(right->get<int>());
                }
                else
                {
                    liveVars[stat].insert(right->get<int>());
                }
            }
            if (prev && !right->right())
            {
                right = prev;
                prev = nullptr;
            }
        }
    }
    bool changed;
    // move down from definitions
    NodeVars reachVars = defVars;
    reachVars.insert(defPhiVars.begin(), defPhiVars.end());
    CodeGraph *graph = createCodeGraph(func);
    do
    {
        changed = false;
        // propagate reachVars
        graph->clearVisited();
        CodeGraph::Node *node = graph->getNodes().front();
        graph->dfs(node, (CodeGraph::Node*)nullptr,
            [&reachVars, &changed]
                (CodeGraph::Node *node, CodeGraph::Node *&prev)
            {
                Tree *t = node->info;
                Tree *pt = prev ? prev->info : nullptr;
                const Vars &reachPrev = reachVars[pt];
                Vars &reach = reachVars[t];
                for (auto var : reachPrev)
                {
                    if (!reach.contains(var))
                    {
                        reach.insert(var);
                        changed = true;
                    }
                }
                prev = node;
            }
        );
    }
    while (changed);
    delete graph;

    // move up from usage
    graph = createReverseCodeGraph(func);
    do
    {
        changed = false;
        // propagate liveVars
        graph->clearVisited();
        CodeGraph::Node *node = graph->getNodes().back();
        graph->dfs(node, (CodeGraph::Node*)nullptr,
            [&defVars, &defPhiVars, &usePhiVars, &liveVars, &reachVars, &changed]
                (CodeGraph::Node *node, CodeGraph::Node *&prev)
            {
                Tree *t = node->info;
                Tree *pt = prev ? prev->info : nullptr;
                Vars add = liveVars[pt];
                Vars remove = defVars[pt];
                const Vars &defsPhi = defPhiVars[pt];
                const Vars &reach = reachVars[t];
                remove.insert(defsPhi.begin(), defsPhi.end());

                Vars &curLive = liveVars[t];
                // if needs phi defs, add phi usages
                for (auto def : defPhiVars[t])
                {
                    if (curLive.contains(def))
                    {
                        const Vars &usePhi = usePhiVars[t];
                        add.insert(usePhi.begin(), usePhi.end());
                        break;
                    }
                }

                for (auto var : remove)
                    add.erase(var);
                for (Vars::iterator it = add.begin() ; it != add.end() ; )
                {
                    if (!reach.contains(*it))
                        it = add.erase(it);
                    else
                        ++it;
                }

                // add prev live to curr live
                for (auto var : add)
                {
                    if (!curLive.contains(var))
                    {
                        curLive.insert(var);
                        changed = true;
                    }
                }
                // remove prev defs from live
                for (auto var : remove)
                {
                    if (curLive.contains(var))
                    {
                        curLive.erase(var);
                        changed = true;
                    }
                }
                prev = node;
            }
        );
    }
    while (changed);
    delete graph;

    // debug output
    std::cout << "=== After liveness analysis:\n";
    for (Tree *stat = func->down() ; stat ; stat = stat->right())
    {
        const Vars &vars = liveVars[stat];
        std::cout << "Live:";
        for (auto var : vars)
            std::cout << " " << var;
        std::cout << "\n";
        stat->print(2);
    }

    // build variable graph
    VariableGraph *varGraph = buildVariableGraph(liveVars);

    // allocate registers
    colorGraph(varGraph);

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
