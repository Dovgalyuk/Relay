#ifndef NODE_H
#define NODE_H

#include <variant>
#include <memory>
#include <list>
#include <cassert>

enum class NodeType
{
    NONE,
    INT,
    VAR,
    FUNC,
    LIST,
    SCOPE,
    ASSIGN,

    // backend nodes
    MOV,
};

class Node;
typedef std::list<Node*> Nodes;

class Node
{
public:
    Node(int v)
        : parent(nullptr), value(v), type(NodeType::INT)
    {}
    Node(char v)
        : parent(nullptr), value((int)v), type(NodeType::VAR)
    {}
    Node(char *v)
        : parent(nullptr), value(v),
          type(NodeType::FUNC)
    {}
    // copy properties, not links
    Node(const Node *n)
        : parent(nullptr), value(n->value), type(n->type)
    {
    }

    Node(NodeType t)
        : parent(nullptr), type(t)
    {
    }

    void setType(NodeType t) { type = t; }
    NodeType getType() const { return type; }
    Nodes &getChildren() { return children; }
    Node *getParent() { return parent; }

    void addFirstChild(Node *n);
    void addChild(Node *n);
    void merge(Node *n);

    std::string getNodeName() const;

    void print() const;

    template <typename T> T get() const { return std::get<T>(value); }

private:
    void printTree(int level) const;

private:
    std::variant<
        int, // integer literal or variable
        std::string // function name
        > value;

    Node *parent;
    NodeType type;
    Nodes children;
};

class NodeIterator
{
public:
    NodeIterator(Node *n)
    {
        root.push_back(n);
        stack.push_back(root.begin());
    }

    Node *operator*()
    {
        if (stack.empty())
            return nullptr;
        if (parent()->getChildren().end() == stack.back())
            return nullptr;
        return current();
    }

    NodeIterator &down()
    {
        stack.push_back(current()->getChildren().begin());
        return *this;
    }
    NodeIterator &right()
    {
        assert(stack.back() != parent()->getChildren().end());
        ++stack.back();
        return *this;
    }
    NodeIterator &operator++()
    {
        // down
        if (!current()->getChildren().empty())
        {
            stack.push_back(current()->getChildren().begin());
            return *this;
        }
        // right and up
        while (!stack.empty())
        {
            auto end = parent()->getChildren().end();
            ++stack.back();
            if (stack.back() == end)
            {
                stack.pop_back();
            }
            else
            {
                return *this;
            }
        }
        return *this;
    }
private:
    Node *current()
    {
        assert(stack.back() != parent()->getChildren().end());
        return *stack.back();
    }
    Node *parent()
    {
        auto it = stack.rbegin();
        assert(it != stack.rend());
        ++it;
        if (it == stack.rend())
            return nullptr;
        return **it;
    }

private:
    Nodes root;
    std::list<Nodes::iterator> stack;
};

#endif
