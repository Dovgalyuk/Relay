#include <iostream>
#include "graph.h"
#include "parser.h"
#include "middleend.h"
#include "backend.h"
#include "codegen.h"
#include "optimization.h"
#include "allocation.h"

Tree *root;

int main(int argc, char *argv[])
{
    if (!yyparse() && root)
    {
        std::cout << "=== Syntax tree:\n";
        root->print();
        // optimize tree
        // convert to 3-address code
        root = makeOperations(root);
        //checkRanges(root);
        std::cout << "=== 3-address tree:\n";
        root->print();
        // add return at the end
        insertReturns(root);
        {
            CodeGraphList *codeGraph = createCodeGraphs(root);
            std::cout << "=== control flow graphs:\n";
            for (auto g : *codeGraph)
            {
                std::cout << "\nGraph " << g << "\n";
                g->print();
            }
            // remote dead code
            removeDeadCode(codeGraph, root);
            delete codeGraph;
        }
        std::cout << "=== After dead code elimination:\n";
        root->print();
        // create variables instead of symbols
        createVariables(root);
        std::cout << "=== Created variables:\n";
        root->print();
        // optimize assignments
        //makeCopyPropagation(root);
        //removeUselessCopying(root);
        //std::cout << "=== After copy propagation:\n";
        //root->print();
        // allocate variables
        simpleAllocation(root);
        removeUselessCopying(root);
        std::cout << "=== After allocation:\n";
        root->print();
        // transform code to instruction level
        root = makeInstructions(root);
        std::cout << "=== Instructions:\n";
        root->print();
        // optimize instructions
        // generate code
        codegen(root);
    }
    else
    {
        printf("Parse error\n");
    }
    return 0;
}
