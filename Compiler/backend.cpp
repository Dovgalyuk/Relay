#include <cassert>
#include "backend.h"

static void processAssign(Tree *func, Tree *op)
{
    Tree *left = op->down()->down();
    Tree *right = op->down()->right()->down();
    while (left && right)
    {
        Tree *mov = new Tree(TreeType::MOV);
        mov->addChild(new Tree(left));
        mov->addChild(new Tree(right));
        func->addChild(mov);
        left = left->right();
        right = right->right();
    }
}

static void processIf(Tree *func, Tree *op)
{
    func->addChild(new Tree(TreeType::JMP,
        op->down()->clone(),
        op->down()->right()->clone()));
}

static void processGoto(Tree *func, Tree *op)
{
    func->addChild(new Tree(TreeType::JMP,
        new Tree(TreeType::ALWAYS),
        op->down()->clone()));
}

static void processAdd(Tree *func, Tree *op)
{
    Tree *res = op->down();
    Tree *left = res->right();
    Tree *right = left->right();
    res = res->down();
    left = left->down();
    right = right->down();
    bool first = true;
    while (res)
    {
        Tree *n1 = left;
        Tree *n2 = right;
        if (n2)
        {
            if (n1 || n1->getType() == TreeType::INT)
            {
                std::swap(n1, n2);
            }
            assert(n1->getType() != TreeType::INT);
        }
        Tree *add = new Tree(first ? TreeType::ADD : TreeType::ADC);
        add->addChild(new Tree(res));
        add->addChild(new Tree(n1));
        if (n2)
        {
            add->addChild(new Tree(n2));
            right = right->right();
        }
        else
        {
            add->addChild(new Tree(0));
        }
        func->addChild(add);
        first = false;

        res = res->right();
        left = left->right();
    }
}

static void processCmp(Tree *func, Tree *op)
{
    Tree *left = op->down();
    Tree* right = left->right();
    left = left->down();
    right = right->down();
    bool first = true;
    while (left || right)
    {
        Tree *n1 = left;
        Tree *n2 = right;
        Tree *sub = new Tree(first ? TreeType::SUB : TreeType::SBC);
        // This is FLAG register
        sub->addChild(new Tree(-1, TreeType::VAR));
        sub->addChild(new Tree(n1));
        if (n2)
        {
            sub->addChild(new Tree(n2));
            right = right->right();
        }
        else
        {
            sub->addChild(new Tree(0));
        }
        func->addChild(sub);
        first = false;

        left = left->right();
    }
}

static void processLShift(Tree *func, Tree *op)
{
    Tree *left = op->down();
    Tree *right = left->right()->down();
    left = left->down();
    bool first = true;
    while (left && right)
    {
        if (left->getType() == TreeType::INT)
        {
            // TODO: support non-zero
        }
        else
        {
            Tree *add = new Tree(first ? TreeType::ADD : TreeType::ADC);
            add->addChild(new Tree(left));
            add->addChild(new Tree(right));
            add->addChild(new Tree(right));
            func->addChild(add);
            first = false;
        }
        left = left->right();
        right = right->right();
    }
}

static void processRShift(Tree *func, Tree *op)
{
    Tree *left = op->down()->down();
    while (left->right())
        left = left->right();
    Tree *right = op->down()->right()->down();
    while (right->right())
        right = right->right();
    while (left && right)
    {
        if (left->getType() == TreeType::INT)
        {
            // TODO: support non-zero
        }
        else
        {
            Tree *shr = new Tree(TreeType::SHR);
            shr->addChild(new Tree(left));
            shr->addChild(new Tree(right));
            func->addChild(shr);
        }
        left = left->left();
        right = right->left();
    }
}

static Tree *processFunction(Tree *func)
{
    Tree *newFunc = new Tree(func);

    Tree *it = func->down();
    while (it)
    {
        switch (it->getType())
        {
        case TreeType::LABEL:
            newFunc->addChild(it->clone());
            break;
        case TreeType::ASSIGN:
            processAssign(newFunc, it);
            break;
        case TreeType::IF:
            processIf(newFunc, it);
            break;
        case TreeType::GOTO:
            processGoto(newFunc, it);
            break;
        case TreeType::CMP:
            processCmp(newFunc, it);
            break;
        case TreeType::RETURN:
            newFunc->addChild(
                new Tree(TreeType::JMP,
                    new Tree(TreeType::ALWAYS),
                    // This is L register
                    new Tree(-1, TreeType::VAR)));
            break;
        case TreeType::ADD:
            processAdd(newFunc, it);
            break;
        case TreeType::LSHIFT:
            processLShift(newFunc, it);
            break;
        case TreeType::RSHIFT:
            processRShift(newFunc, it);
            break;
        default:
            break;
        }
        it = it->right();
    }

    return newFunc;
}

Tree *makeInstructions(Tree *root)
{
    Tree *newRoot = new Tree(TreeType::LIST);
    // iterate over functions
    Tree *it = root->down();
    while (it)
    {
        newRoot->addChild(processFunction(it));
        it = it->right();
    }
    delete root;
    return newRoot;
}
