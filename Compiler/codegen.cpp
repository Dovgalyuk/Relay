#include <iostream>
#include <cassert>
#include "codegen.h"

static void genLabel(Tree *op)
{
    std::cout << "Label" << op->get<int>() << ":\n";
}

static void genAssign(Tree *op)
{
    std::cout << "MOV "
        << op->down()->getTreeName()
        << " " << op->down()->right()->getTreeName() << "\n";
}

static void genJmp(Tree *op)
{
    std::cout << "JMP "
        << op->down()->getTreeName()
        << " " << op->down()->right()->getTreeName() << "\n";
}

static void genBinary(Tree *op)
{
    std::cout << op->getTreeName()
        << " " << op->down()->getTreeName()
        << " " << op->down()->right()->getTreeName()
        << " " << op->down()->right()->right()->getTreeName() << "\n";
}

static void genUnary(Tree *op)
{
    std::cout << op->getTreeName()
        << " " << op->down()->getTreeName()
        << " " << op->down()->right()->getTreeName() << "\n";
}

static void genFunc(Tree *f)
{
    std::cout << f->get<std::string>() << ":\n";
    Tree *op = f->down();
    while (op)
    {
        switch (op->getType())
        {
        case TreeType::LABEL:
            genLabel(op);
            break;
        case TreeType::MOV:
            genAssign(op);
            break;
        case TreeType::JMP:
            genJmp(op);
            break;
        case TreeType::ADC:
        case TreeType::ADD:
        case TreeType::SBC:
        case TreeType::SUB:
            genBinary(op);
            break;
        case TreeType::SHR:
            genUnary(op);
            break;
        default:
            assert(false);
        }
        op = op->right();
    }
}

void codegen(Tree *root)
{
    std::cout << "Generated code:\n";
    Tree *f = root->down();
    while (f)
    {
        assert(f->getType() == TreeType::FUNC);
        genFunc(f);
        f = f->right();
    }
}
