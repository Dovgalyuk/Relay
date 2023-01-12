#ifndef NODE_H
#define NODE_H

#include <variant>
#include <memory>
#include <list>
#include <cassert>

enum class TreeType
{
    NONE,
    INT,
    VAR,
    FUNC,
    LABEL,
    LIST,
    SCOPE,
    ASSIGN,
    RETURN,
    IF,
    LSHIFT,
    RSHIFT,
    ALWAYS,
    CARRY,
    NCARRY,
    ZERO,
    NZERO,
    SIGN,
    NSIGN,
    // same as binary instructions
    // ADD, SUB, OR, XOR, AND

    ////////////////
    // backend nodes
    ////////////////
    MOV,
    JMP,
    // binary ops
    ADC,
    ADD,
    SBC,
    SUB,
    AND,
    OR,
    XOR,
    // unary ops
    SHR,
};

class Tree;
typedef std::list<Tree*> Trees;

class Tree
{
public:
    explicit Tree(int v, TreeType t = TreeType::INT)
        : parent(nullptr), value(v), type(t)
    {}
    explicit Tree(char v)
        : parent(nullptr), value((int)v), type(TreeType::VAR)
    {}
    explicit Tree(char *v)
        : parent(nullptr), value(v),
          type(TreeType::FUNC)
    {}
    // copy properties, not links
    explicit Tree(const Tree *n)
        : parent(nullptr), value(n->value), type(n->type)
    {
    }

    Tree(TreeType t, Tree *n1 = nullptr, Tree *n2 = nullptr, Tree *n3 = nullptr)
        : parent(nullptr), type(t)
    {
        if (n1)
        {
            addChild(n1);
        }
        if (n2)
        {
            addChild(n2);
        }
        if (n3)
        {
            addChild(n3);
        }
    }

    Tree *clone();

    void setType(TreeType t) { type = t; }
    TreeType getType() const { return type; }
    Trees &getChildren() { return children; }
    Tree *getParent() { return parent; }

    void addFirstChild(Tree *n);
    void addChild(Tree *n);
    void merge(Tree *n);

    std::string getTreeName() const;

    void print(int level = 0) const;

    template <typename T> T get() const { return std::get<T>(value); }

private:
    void printTree(int level) const;

private:
    std::variant<
        int, // integer literal or variable
        std::string // function name
        > value;

    Tree *parent;
    TreeType type;
    Trees children;
};

class TreeIterator
{
public:
    TreeIterator(Tree *n)
    {
        root.push_back(n);
        if (n)
        {
            stack.push_back(root.begin());
        }
    }

    Tree *operator*()
    {
        if (stack.empty())
            return nullptr;
        if (parent()->getChildren().end() == stack.back())
            return nullptr;
        return current();
    }

    TreeIterator &up()
    {
        stack.pop_back();
        return *this;
    }
    TreeIterator &down()
    {
        stack.push_back(current()->getChildren().begin());
        return *this;
    }
    TreeIterator &right()
    {
        assert(stack.back() != parent()->getChildren().end());
        ++stack.back();
        return *this;
    }
    TreeIterator &left()
    {
        if (stack.back() == parent()->getChildren().begin())
        {
            // end of iteration, clear the stack
            stack.clear();
        }
        else
        {
            --stack.back();
        }
        return *this;
    }
    TreeIterator &operator++()
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
    Tree *current()
    {
        assert(stack.back() != parent()->getChildren().end());
        return *stack.back();
    }
    Tree *parent()
    {
        auto it = stack.rbegin();
        assert(it != stack.rend());
        ++it;
        if (it == stack.rend())
            return nullptr;
        return **it;
    }

private:
    Trees root;
    std::list<Trees::iterator> stack;
};

#endif
