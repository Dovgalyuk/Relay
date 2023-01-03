#include <iostream>
#include "node.h"

void Node::addFirstChild(Node *n)
{
    children.push_front(n);
    n->parent = this;
}

void Node::addChild(Node *n)
{
    children.push_back(n);
    n->parent = this;
}

void Node::printTree(int level) const
{
    std::cout << std::string(level, ' ')
        << getNodeName() << '\n';
    for (auto c : children)
    {
        c->printTree(level + 2);
    }
}

void Node::merge(Node *n)
{
    for (auto c : n->children)
    {
        addChild(c);
    }
    n->children.clear();
    delete n;
}

void Node::print() const
{
    printTree(0);
}

std::string Node::getNodeName() const
{
    switch (type)
    {
    case NodeType::NONE:
        return "None";
    case NodeType::INT:
        return "Int " + std::to_string(std::get<int>(value));
    case NodeType::VAR:
        return "Var" + std::to_string(std::get<int>(value));
    case NodeType::FUNC:
        return "Function " + std::get<std::string>(value);
    case NodeType::ASSIGN:
        return "Assign";
    case NodeType::LIST:
        return "()";
    case NodeType::SCOPE:
        return "{}";

    case NodeType::MOV:
        return "MOV";
    }
    return std::to_string((int)type);
}
