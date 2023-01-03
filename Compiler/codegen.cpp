#include <iostream>
#include <cassert>
#include "codegen.h"

static void genAssign(Node *op)
{
    auto it = op->getChildren().begin();
    Node *left = *it++;
    Node *right = *it;
    std::cout << "MOV "
        << left->getNodeName()
        << " " << right->getNodeName() << "\n";
}

static void genFunc(Node *f)
{
    std::cout << f->get<std::string>() << ":\n";
    NodeIterator op(f);
    op.down();
    while (*op)
    {
        switch ((*op)->getType())
        {
        case NodeType::MOV:
            genAssign(*op);
            break;
        default:
            assert(false);
        }
        op.right();
    }
}

void codegen(Node *node)
{
    std::cout << "Generated code:\n";
    NodeIterator f(node);
    f.down();
    while (*f)
    {
        assert((*f)->getType() == NodeType::FUNC);
        genFunc(*f);
        f.right();
    }
}
