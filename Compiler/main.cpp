#include <stdio.h>
#include "parser.h"
#include "backend.h"
#include "codegen.h"

Node *root;

int main(int argc, char *argv[])
{
    if (!yyparse() && root)
    {
        root->print();
        // optimize tree
        // create scopes
        // convert to 3-address code
        // allocate variables
        // transform code to instruction level
        root = makeInstructions(root);
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
