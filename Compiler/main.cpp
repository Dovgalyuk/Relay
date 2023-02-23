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
        std::cout << "=== 3-address tree:\n";
        root->print();
        // create scopes and variables and convert to SSA
        CodeGraphList *codeGraph = createCodeGraphs(root);
        std::cout << "=== control flow graphs:\n";
        for (auto g : *codeGraph)
        {
            std::cout << "\nGraph " << g << "\n";
            g->print();
        }
        createVariables(codeGraph, root);
        std::cout << "=== Created variables:\n";
        root->print();
        // optimize assignments
        // makeCopyPropagation(root);
        // std::cout << "=== After copy propagation:\n";
        // root->print();
        // remove dead code
        // allocate variables
        simpleAllocation(root);
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
