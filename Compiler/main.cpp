#include <iostream>
#include "graph.h"
#include "parser.h"
#include "middleend.h"
#include "backend.h"
#include "codegen.h"
#include "optimization.h"

Tree *root;

int main(int argc, char *argv[])
{
    if (!yyparse() && root)
    {
        std::cout << "=== Syntax tree:\n";
        root->print();
        // optimize tree
        // create scopes
        // convert to 3-address code
        root = makeOperations(root);
        std::cout << "=== 3-address tree:\n";
        root->print();
        // convert to SSA
        // optimize assignments
        CodeGraphList *codeGraph = createCodeGraph(root);
        std::cout << "=== control flow graphs:\n";
        for (auto g : *codeGraph)
        {
            std::cout << "\nGraph " << g << "\n";
            g->print();
        }
        makeCopyPropagation(codeGraph);
        // remove dead code
        // allocate variables
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
