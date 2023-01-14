#include <iostream>
#include "tree.h"

Tree *Tree::clone()
{
    Tree *res = new Tree(this);
    Tree *c = child;
    while (c)
    {
        res->addChild(c->clone());
        c = c->next;
    }
    return res;
}

void Tree::addFirstChild(Tree *n)
{
    n->prev = nullptr;
    n->parent = this;
    if (child)
    {
        child->prev = n;
    }
    n->next = child;
    child = n;
}

void Tree::addChild(Tree *n)
{
    n->parent = this;
    n->next = nullptr;
    n->prev = nullptr;
    if (child)
    {
        Tree *last = child;
        while (last->next)
        {
            last = last->next;
        }
        last->next = n;
        n->prev = last;
    }
    else
    {
        child = n;
    }
}

void Tree::printTree(int level) const
{
    std::cout << std::string(level, ' ')
        << getTreeName() << '\n';
    Tree *c = child;
    while (c)
    {
        c->printTree(level + 2);
        c = c->next;
    }
}

void Tree::merge(Tree *n)
{
    Tree *c = n->child;
    while (c)
    {
        Tree *next = c->next;
        addChild(c);
        c = next;
    }
    n->child = nullptr;
    delete n;
}

void Tree::insertParent(Tree *n)
{
    replaceWith(n);
    n->addChild(this);
}

void Tree::insertRight(Tree *n)
{
    if (next)
    {
        next->prev = n;
    }
    n->next = next;
    n->prev = this;
    n->parent = parent;
    next = n;
}

void Tree::replaceWith(Tree *n)
{
    n->parent = parent;
    if (parent->child == this)
    {
        parent->child = n;
    }
    parent = nullptr;
    n->next = next;
    n->prev = prev;
    if (next)
    {
        next->prev = n;
    }
    if (prev)
    {
        prev->next = n;
    }
    next = nullptr;
    prev = nullptr;
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
    case TreeType::SYMBOL:
        return "Sym" + std::to_string(std::get<int>(value));
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
    case TreeType::PHI:
        return "Phi";

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

    case TreeType::REG:
        return std::string(1, (char)get<int>());
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
