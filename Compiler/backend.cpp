#include "backend.h"

static void processAssign(Node *func, Node *op)
{
    NodeIterator left(op);
    left.down().down();
    NodeIterator right(op);
    right.down().right().down();
    while (*left && *right)
    {
        Node *mov = new Node(NodeType::MOV);
        mov->addChild(new Node(*left));
        mov->addChild(new Node(*right));
        func->addChild(mov);
        left = left.right();
        right = right.right();
    }
}

static Node *processFunction(Node *func)
{
    Node *newFunc = new Node(func);

    NodeIterator it(func);
    it.down().down();
    while (*it)
    {
        switch ((*it)->getType())
        {
        case NodeType::ASSIGN:
            processAssign(newFunc, *it);
            break;
        default:
            break;
        }
        it.right();
    }

    return newFunc;
}

Node *makeInstructions(Node *root)
{
    Node *newRoot = new Node(NodeType::LIST);
    // iterate over functions
    NodeIterator it(root);
    it.down();
    while (*it)
    {
        newRoot->addChild(processFunction(*it));
        it.right();
    }
    delete root;
    return newRoot;
}
