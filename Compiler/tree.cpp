#include <iostream>
#include "tree.h"

Tree *Tree::clone()
{
    Tree *res = new Tree(this);
    for (auto c : children)
    {
        res->addChild(c->clone());
    }
    return res;
}

void Tree::addFirstChild(Tree *n)
{
    children.push_front(n);
    n->parent = this;
}

void Tree::addChild(Tree *n)
{
    children.push_back(n);
    n->parent = this;
}

void Tree::printTree(int level) const
{
    std::cout << std::string(level, ' ')
        << getTreeName() << '\n';
    for (auto c : children)
    {
        c->printTree(level + 2);
    }
}

void Tree::merge(Tree *n)
{
    for (auto c : n->children)
    {
        addChild(c);
    }
    n->children.clear();
    delete n;
}

void Tree::print(int level) const
{
    printTree(level);
}

std::string Tree::getTreeName() const
{
    switch (type)
    {
    case TreeType::NONE:
        return "None";
    case TreeType::INT:
        return "Int " + std::to_string(std::get<int>(value));
    case TreeType::VAR:
        return "Var" + std::to_string(std::get<int>(value));
    case TreeType::FUNC:
        return "Function " + std::get<std::string>(value);
    case TreeType::LABEL:
        return "Label" + std::to_string(std::get<int>(value));
    case TreeType::LIST:
        return "()";
    case TreeType::SCOPE:
        return "{}";

    case TreeType::ASSIGN:
        return "Assign";
    case TreeType::RETURN:
        return "Return";
    case TreeType::IF:
        return "If";
    case TreeType::WHILE:
        return "While";
    case TreeType::GOTO:
        return "Goto";
    case TreeType::CMP:
        return "Cmp";
    case TreeType::LSHIFT:
        return "LShift";
    case TreeType::RSHIFT:
        return "RShift";

    case TreeType::ALWAYS:
        return "ALWAYS";
    case TreeType::CARRY:
        return "CARRY";
    case TreeType::NCARRY:
        return "NCARRY";
    case TreeType::ZERO:
        return "ZERO";
    case TreeType::NZERO:
        return "NZERO";
    case TreeType::SIGN:
        return "SIGN";
    case TreeType::NSIGN:
        return "NSIGN";
    case TreeType::EQUAL:
        return "EQUAL";
    case TreeType::NEQUAL:
        return "NEQUAL";

    case TreeType::MOV:
        return "MOV";
    case TreeType::JMP:
        return "JMP";
    case TreeType::ADC:
        return "ADC";
    case TreeType::ADD:
        return "ADD";
    case TreeType::SBC:
        return "SBC";
    case TreeType::SUB:
        return "SUB";
    case TreeType::SHR:
        return "SHR";
    }
    return std::to_string((int)type);
}
