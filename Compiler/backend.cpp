#include <cassert>
#include "backend.h"

static void processAssign(Tree *func, Tree *op)
{
    TreeIterator left(op);
    left.down().down();
    TreeIterator right(op);
    right.down().right().down();
    while (*left && *right)
    {
        Tree *mov = new Tree(TreeType::MOV);
        mov->addChild(new Tree(*left));
        mov->addChild(new Tree(*right));
        func->addChild(mov);
        left = left.right();
        right = right.right();
    }
}

static void processIf(Tree *func, Tree *op)
{
    TreeIterator left(op);
    left.down();
    TreeIterator right(left);
    right.right();
    func->addChild(new Tree(TreeType::JMP, (*left)->clone(), (*right)->clone()));
}

static void processAdd(Tree *func, Tree *op)
{
    TreeIterator res(op);
    res.down();
    TreeIterator left(res);
    left.right();
    TreeIterator right(left);
    right.right();
    res.down();
    left.down();
    right.down();
    bool first = true;
    while (*res)
    {
        Tree *n1 = *left;
        Tree *n2 = *right;
        if (n2)
        {
            if (n1 || n1->getType() == TreeType::INT)
            {
                std::swap(n1, n2);
            }
            assert(n1->getType() != TreeType::INT);
        }
        Tree *add = new Tree(first ? TreeType::ADD : TreeType::ADC);
        add->addChild(new Tree(*res));
        add->addChild(new Tree(n1));
        if (n2)
        {
            add->addChild(new Tree(n2));
            right.right();
        }
        else
        {
            add->addChild(new Tree(0));
        }
        func->addChild(add);
        first = false;

        res.right();
        left.right();
    }
}

static void processLShift(Tree *func, Tree *op)
{
    TreeIterator left(op);
    left.down().down();
    bool first = true;
    while (*left)
    {
        if ((*left)->getType() == TreeType::INT)
        {
            // TODO: support non-zero
        }
        else
        {
            Tree *add = new Tree(first ? TreeType::ADD : TreeType::ADC);
            add->addChild(new Tree(*left));
            add->addChild(new Tree(*left));
            add->addChild(new Tree(*left));
            func->addChild(add);
            first = false;
        }
        left.right();
    }
}

static void processRShift(Tree *func, Tree *op)
{
    TreeIterator left(op);
    left.down().down();
    while (*left)
        left.right();
    left.left();
    while (*left)
    {
        if ((*left)->getType() == TreeType::INT)
        {
            // TODO: support non-zero
        }
        else
        {
            Tree *shr = new Tree(TreeType::SHR);
            shr->addChild(new Tree(*left));
            shr->addChild(new Tree(*left));
            func->addChild(shr);
        }
        left.left();
    }
}

static Tree *processFunction(Tree *func)
{
    Tree *newFunc = new Tree(func);

    TreeIterator it(func);
    it.down();
    while (*it)
    {
        switch ((*it)->getType())
        {
        case TreeType::LABEL:
            newFunc->addChild(*it);
            break;
        case TreeType::ASSIGN:
            processAssign(newFunc, *it);
            break;
        case TreeType::IF:
            processIf(newFunc, *it);
            break;
        case TreeType::RETURN:
            // do nothing
            break;
        case TreeType::ADD:
            processAdd(newFunc, *it);
            break;
        case TreeType::LSHIFT:
            processLShift(newFunc, *it);
            break;
        case TreeType::RSHIFT:
            processRShift(newFunc, *it);
            break;
        default:
            break;
        }
        it.right();
    }

    return newFunc;
}

Tree *makeInstructions(Tree *root)
{
    Tree *newRoot = new Tree(TreeType::LIST);
    // iterate over functions
    TreeIterator it(root);
    it.down();
    while (*it)
    {
        newRoot->addChild(processFunction(*it));
        it.right();
    }
    delete root;
    return newRoot;
}
