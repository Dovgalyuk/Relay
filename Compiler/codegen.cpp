#include <iostream>
#include <cassert>
#include "codegen.h"

static void genLabel(Tree *op)
{
    std::cout << "Label" << op->get<int>() << ":\n";
}

static void genAssign(Tree *op)
{
    auto it = op->getChildren().begin();
    Tree *left = *it++;
    Tree *right = *it;
    std::cout << "MOV "
        << left->getTreeName()
        << " " << right->getTreeName() << "\n";
}

static void genJmp(Tree *op)
{
    TreeIterator it(op);
    it.down();
    Tree *first = *it;
    it.right();
    Tree *second = *it;
    std::cout << "JMP "
        << first->getTreeName()
        << " " << second->getTreeName() << "\n";
}

static void genBinary(Tree *op)
{
    TreeIterator it(op);
    it.down();
    Tree *first = *it;
    it.right();
    Tree *second = *it;
    it.right();
    Tree *third = *it;
    std::cout << op->getTreeName()
        << " " << first->getTreeName()
        << " " << second->getTreeName()
        << " " << third->getTreeName() << "\n";
}

static void genUnary(Tree *op)
{
    TreeIterator it(op);
    it.down();
    Tree *first = *it;
    it.right();
    Tree *second = *it;
    std::cout << op->getTreeName()
        << " " << first->getTreeName()
        << " " << second->getTreeName() << "\n";
}

static void genFunc(Tree *f)
{
    std::cout << f->get<std::string>() << ":\n";
    TreeIterator op(f);
    op.down();
    while (*op)
    {
        switch ((*op)->getType())
        {
        case TreeType::LABEL:
            genLabel(*op);
            break;
        case TreeType::MOV:
            genAssign(*op);
            break;
        case TreeType::JMP:
            genJmp(*op);
            break;
        case TreeType::ADC:
        case TreeType::ADD:
        case TreeType::SBC:
        case TreeType::SUB:
            genBinary(*op);
            break;
        case TreeType::SHR:
            genUnary(*op);
            break;
        default:
            assert(false);
        }
        op.right();
    }
}

void codegen(Tree *root)
{
    std::cout << "Generated code:\n";
    TreeIterator f(root);
    f.down();
    while (*f)
    {
        assert((*f)->getType() == TreeType::FUNC);
        genFunc(*f);
        f.right();
    }
}
